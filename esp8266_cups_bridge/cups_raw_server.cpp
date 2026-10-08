#include "cups_raw_server.h"
#include "config.h"

// TCP Servers for port 9100 (JetDirect / RAW) and port 515 (LPD)
static WiFiServer rawServer(RAW_JETDIRECT_PORT);
static WiFiServer lpdServer(LPD_PORT);

static WiFiClient rawClient;
static WiFiClient lpdClient;

// Ring buffer for streaming network bytes to serial
static uint8_t  stream_buffer[STREAM_BUFFER_SIZE];
static uint16_t stream_head = 0;
static uint16_t stream_tail = 0;

// Flow control state from Arduino
static bool     serial_flow_allowed = true;
static uint32_t total_bytes_printed = 0;

// Printer status string from Arduino
static String   printer_status_str = "READY";
static bool     printer_paper_out  = false;
static bool     printer_fault      = false;
static bool     printer_busy       = false;

// Line buffer for reading Arduino text messages
static String   arduino_line = "";

// LPD Protocol state
enum LpdState {
    LPD_WAIT_CMD,
    LPD_WAIT_CTRL_CMD,
    LPD_RECV_CTRL_DATA,
    LPD_WAIT_DATA_CMD,
    LPD_RECV_DATA_PAYLOAD
};
static LpdState lpd_state = LPD_WAIT_CMD;
static size_t   lpd_expected_bytes = 0;
static size_t   lpd_received_bytes = 0;

static inline uint16_t buffer_count() {
    return (stream_head - stream_tail + STREAM_BUFFER_SIZE) % STREAM_BUFFER_SIZE;
}

static inline uint16_t buffer_free() {
    return (STREAM_BUFFER_SIZE - 1) - buffer_count();
}

void raw_server_init() {
    rawServer.begin();
    rawServer.setNoDelay(true);

    lpdServer.begin();
    lpdServer.setNoDelay(true);

    stream_head = 0;
    stream_tail = 0;
    serial_flow_allowed = true;
    total_bytes_printed = 0;
}

bool raw_server_push_byte(uint8_t b) {
    if (buffer_free() == 0) {
        return false;
    }
    stream_buffer[stream_head] = b;
    stream_head = (stream_head + 1) % STREAM_BUFFER_SIZE;
    return true;
}

bool raw_server_push_buffer(const uint8_t *data, size_t len) {
    if (buffer_free() < len) {
        return false;
    }
    for (size_t i = 0; i < len; i++) {
        stream_buffer[stream_head] = data[i];
        stream_head = (stream_head + 1) % STREAM_BUFFER_SIZE;
    }
    return true;
}

String raw_server_get_printer_status() {
    return printer_status_str;
}

bool raw_server_is_paper_out() {
    return printer_paper_out;
}

bool raw_server_is_busy() {
    return printer_busy;
}

bool raw_server_is_fault() {
    return printer_fault;
}

uint32_t raw_server_get_bytes_printed() {
    return total_bytes_printed;
}

uint16_t raw_server_get_buffer_fill() {
    return buffer_count();
}

bool raw_server_is_flow_allowed() {
    return serial_flow_allowed;
}

bool raw_server_is_job_active() {
    return ((rawClient && rawClient.connected()) ||
            (lpdClient && lpdClient.connected()) ||
            (buffer_count() > 0));
}

void raw_server_send_command(const String &cmd) {
    Serial.println(cmd);
}

static void parse_arduino_message(const String &line) {
    if (line.startsWith("STATUS:")) {
        printer_status_str = line.substring(7);
        printer_status_str.trim();

        printer_paper_out = (line.indexOf("PAPER_OUT") >= 0) || (line.indexOf("PE=1") >= 0);
        printer_fault     = (line.indexOf("FAULT") >= 0 && line.indexOf("FAULT=0") < 0);
        printer_busy      = (line.indexOf("BUSY=1") >= 0);
    }
}

// Service LPD state machine for port 515
static void process_lpd_client() {
    if (!lpdClient || !lpdClient.connected()) {
        if (lpdServer.hasClient()) {
            lpdClient = lpdServer.available();
            lpd_state = LPD_WAIT_CMD;
        }
        return;
    }

    while (lpdClient.available() > 0) {
        switch (lpd_state) {
            case LPD_WAIT_CMD: {
                // Read queue command line: e.g. \x02printer_queue\n
                String cmd = lpdClient.readStringUntil('\n');
                // Reply with 0x00 ACK (success)
                lpdClient.write((uint8_t)0x00);
                lpd_state = LPD_WAIT_CTRL_CMD;
                break;
            }
            case LPD_WAIT_CTRL_CMD: {
                // Command: \x02<len> cfA...\n or \x03<len> dfA...\n
                String subcmd = lpdClient.readStringUntil('\n');
                if (subcmd.length() == 0) break;
                char type = subcmd.charAt(0);
                int spaceIdx = subcmd.indexOf(' ');
                int nextSpace = subcmd.indexOf(' ', spaceIdx + 1);
                size_t len = 0;
                if (spaceIdx > 0) {
                    if (nextSpace > spaceIdx) {
                        len = subcmd.substring(spaceIdx + 1, nextSpace).toInt();
                    } else {
                        len = subcmd.substring(1, spaceIdx).toInt();
                    }
                }
                lpd_expected_bytes = len;
                lpd_received_bytes = 0;
                lpdClient.write((uint8_t)0x00); // ACK

                if (type == '\x02') {
                    lpd_state = LPD_RECV_CTRL_DATA;
                } else if (type == '\x03') {
                    lpd_state = LPD_RECV_DATA_PAYLOAD;
                }
                break;
            }
            case LPD_RECV_CTRL_DATA: {
                // Read and discard control file bytes
                while (lpdClient.available() && lpd_received_bytes < lpd_expected_bytes) {
                    lpdClient.read();
                    lpd_received_bytes++;
                }
                if (lpd_received_bytes >= lpd_expected_bytes) {
                    // Check trailing 0x00
                    if (lpdClient.available() && lpdClient.peek() == 0x00) {
                        lpdClient.read();
                    }
                    lpdClient.write((uint8_t)0x00); // ACK
                    lpd_state = LPD_WAIT_CTRL_CMD;
                }
                break;
            }
            case LPD_RECV_DATA_PAYLOAD: {
                // Read data payload bytes directly into stream buffer
                while (lpdClient.available() && lpd_received_bytes < lpd_expected_bytes && buffer_free() > 0) {
                    uint8_t b = lpdClient.read();
                    raw_server_push_byte(b);
                    lpd_received_bytes++;
                }
                if (lpd_received_bytes >= lpd_expected_bytes) {
                    if (lpdClient.available() && lpdClient.peek() == 0x00) {
                        lpdClient.read();
                    }
                    lpdClient.write((uint8_t)0x00); // ACK
                    lpd_state = LPD_WAIT_CTRL_CMD;
                }
                // Backpressure: if buffer is full, break to allow draining
                if (buffer_free() == 0) return;
                break;
            }
        }
    }
}

void raw_server_loop() {
    // ------------------------------------------------------------------------
    // 1. Check for incoming Serial responses from Arduino (XON/XOFF and text)
    // ------------------------------------------------------------------------
    while (Serial.available()) {
        uint8_t c = Serial.read();

        if (c == FLOW_XOFF) {
            // Arduino buffer is full, pause transmission
            serial_flow_allowed = false;
        } else if (c == FLOW_XON) {
            // Arduino buffer drained, resume transmission
            serial_flow_allowed = true;
        } else {
            // Text status message
            if (c == '\n' || c == '\r') {
                if (arduino_line.length() > 0) {
                    parse_arduino_message(arduino_line);
                    arduino_line = "";
                }
            } else if (c >= 32 && c <= 126) {
                if (arduino_line.length() < 128) {
                    arduino_line += (char)c;
                }
            }
        }
    }

    // ------------------------------------------------------------------------
    // 2. Handle Port 9100 RAW JetDirect TCP Client
    // ------------------------------------------------------------------------
    static unsigned long raw_last_activity = 0;

    if (!rawClient || !rawClient.connected()) {
        if (rawServer.hasClient()) {
            rawClient = rawServer.available();
            raw_last_activity = millis();
        }
    }

    if (rawClient && rawClient.connected()) {
        // Read incoming network bytes as long as stream buffer has room
        while (rawClient.available() > 0 && buffer_free() > 0) {
            uint8_t b = rawClient.read();
            raw_server_push_byte(b);
            raw_last_activity = millis();
        }
        // If stream buffer is full, stop reading from rawClient.
        // TCP window throttling will automatically pause CUPS upstream!

        // Release socket after 8 seconds of idle silence so other devices can print
        if (rawClient.available() == 0 && (millis() - raw_last_activity > 8000)) {
            rawClient.stop();
        }
    }

    // ------------------------------------------------------------------------
    // 3. Handle Port 515 LPD Client
    // ------------------------------------------------------------------------
    process_lpd_client();

    // ------------------------------------------------------------------------
    // 4. Drain stream buffer to Serial towards Arduino (if flow is allowed)
    // ------------------------------------------------------------------------
    while (serial_flow_allowed && stream_head != stream_tail && Serial.availableForWrite() > 0) {
        uint8_t b = stream_buffer[stream_tail];
        stream_tail = (stream_tail + 1) % STREAM_BUFFER_SIZE;
        Serial.write(b);
        total_bytes_printed++;
    }
}
