#include "web_portal.h"
#include "cups_raw_server.h"
#include "config.h"
#include <ESP8266WebServer.h>

static ESP8266WebServer server(HTTP_PORT);

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Amstrad DMP3000 Print Server</title>
  <style>
    :root {
      --bg: #1e1e24;
      --card-bg: #2b2b36;
      --accent: #4ade80;
      --accent-hover: #22c55e;
      --text: #f3f4f6;
      --text-muted: #9ca3af;
      --border: #3f3f50;
      --danger: #ef4444;
      --warning: #f59e0b;
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", sans-serif;
      background: var(--bg);
      color: var(--text);
      margin: 0;
      padding: 1rem;
      display: flex;
      justify-content: center;
    }
    .container {
      width: 100%;
      max-width: 720px;
    }
    header {
      text-align: center;
      margin-bottom: 1.5rem;
      border-bottom: 1px solid var(--border);
      padding-bottom: 1rem;
    }
    header h1 {
      margin: 0;
      font-size: 1.6rem;
      color: var(--accent);
      letter-spacing: 0.05em;
    }
    header p {
      margin: 0.25rem 0 0;
      color: var(--text-muted);
      font-size: 0.9rem;
    }
    .card {
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: 8px;
      padding: 1.25rem;
      margin-bottom: 1.25rem;
    }
    .card h2 {
      margin-top: 0;
      font-size: 1.15rem;
      border-bottom: 1px solid var(--border);
      padding-bottom: 0.5rem;
      color: #fff;
    }
    .badge-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(130px, 1fr));
      gap: 0.75rem;
      margin-top: 0.75rem;
    }
    .badge {
      background: #181820;
      border-radius: 6px;
      padding: 0.6rem;
      text-align: center;
      border: 1px solid var(--border);
    }
    .badge .label {
      font-size: 0.75rem;
      color: var(--text-muted);
      text-transform: uppercase;
      letter-spacing: 0.05em;
    }
    .badge .val {
      font-size: 1.1rem;
      font-weight: bold;
      margin-top: 0.2rem;
    }
    .val.ok { color: var(--accent); }
    .val.warn { color: var(--warning); }
    .val.err { color: var(--danger); }
    textarea {
      width: 100%;
      box-sizing: border-box;
      height: 140px;
      background: #181820;
      border: 1px solid var(--border);
      color: #e5e7eb;
      font-family: monospace;
      font-size: 0.95rem;
      padding: 0.75rem;
      border-radius: 6px;
      resize: vertical;
    }
    .options-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
      gap: 0.5rem;
      margin: 1rem 0;
    }
    label {
      font-size: 0.85rem;
      display: flex;
      align-items: center;
      gap: 0.4rem;
      cursor: pointer;
    }
    .btn {
      background: var(--accent);
      color: #0d1f12;
      font-weight: 600;
      border: none;
      padding: 0.65rem 1.25rem;
      border-radius: 6px;
      cursor: pointer;
      font-size: 0.95rem;
      transition: background 0.15s ease;
    }
    .btn:hover { background: var(--accent-hover); }
    .btn-secondary {
      background: #374151;
      color: #fff;
    }
    .btn-secondary:hover { background: #4b5563; }
    .btn-row {
      display: flex;
      flex-wrap: wrap;
      gap: 0.5rem;
      margin-top: 0.5rem;
    }
    .alert {
      padding: 0.75rem;
      border-radius: 6px;
      margin-bottom: 1rem;
      font-size: 0.85rem;
      display: none;
    }
    .alert-success { background: #14532d; color: #86efac; border: 1px solid #166534; }
    .instructions {
      font-size: 0.85rem;
      color: var(--text-muted);
      line-height: 1.5;
    }
    .instructions code {
      background: #181820;
      padding: 0.15rem 0.35rem;
      border-radius: 4px;
      color: #cbd5e1;
    }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <h1>AMSTRAD DMP3000</h1>
      <p>Wi-Fi CUPS & Raw Print Server (ESP8266 + Arduino Bridge)</p>
    </header>

    <div id="alertBox" class="alert alert-success"></div>

    <div class="card">
      <h2>Live Printer Status</h2>
      <div class="badge-grid">
        <div class="badge">
          <div class="label">Printer State</div>
          <div id="badgeStatus" class="val ok">ONLINE</div>
        </div>
        <div class="badge">
          <div class="label">Paper</div>
          <div id="badgePaper" class="val ok">LOADED</div>
        </div>
        <div class="badge">
          <div class="label">Buffer Queue</div>
          <div id="badgeBuffer" class="val">0 B</div>
        </div>
        <div class="badge">
          <div class="label">Total Printed</div>
          <div id="badgePrinted" class="val">0 B</div>
        </div>
      </div>
    </div>

    <div class="card">
      <h2>Direct Text Print (Android / Mac / Browser)</h2>
      <form id="printForm">
        <textarea name="text" id="printText" placeholder="Type or paste text to print directly here..."></textarea>
        
        <div class="options-grid">
          <label><input type="checkbox" name="nlq" value="1"> Near Letter Quality (NLQ)</label>
          <label><input type="checkbox" name="bold" value="1"> Bold (Emphasized)</label>
          <label><input type="checkbox" name="condensed" value="1"> Condensed (17 CPI)</label>
          <label><input type="checkbox" name="elite" value="1"> Elite (12 CPI)</label>
          <label><input type="checkbox" name="dwidth" value="1"> Double Width</label>
          <label><input type="checkbox" name="eject" value="1" checked> Form Feed on finish</label>
        </div>

        <button type="submit" class="btn">Print Text Now</button>
      </form>
    </div>

    <div class="card">
      <h2>Quick Actions</h2>
      <div class="btn-row">
        <button class="btn btn-secondary" onclick="triggerAction('/testpage')">Print Test Page</button>
        <button class="btn btn-secondary" onclick="triggerAction('/eject')">Form Feed (Eject)</button>
        <button class="btn btn-secondary" onclick="triggerAction('/reset')">Reset Printer</button>
      </div>
    </div>

    <div class="card">
      <h2>Connection Details</h2>
      <div class="instructions">
        <p><strong>CUPS (macOS / Linux):</strong> Add printer using AppSocket / JetDirect URI: <code>socket://oldprinter.local:9100</code> or <code>lpd://oldprinter.local/raw</code>. Select driver <em>Generic Text-Only</em> or <em>IBM Proprinter</em>.</p>
        <p><strong>Android:</strong> Print directly from this web page, or use print apps supporting port 9100 (e.g. <em>RawBT</em> or <em>PrintBot</em> targeting <code><span id="ipPlaceholder"></span>:9100</code>).</p>
      </div>
    </div>
  </div>

  <script>
    function showAlert(msg) {
      const box = document.getElementById('alertBox');
      box.innerText = msg;
      box.style.display = 'block';
      setTimeout(() => { box.style.display = 'none'; }, 4000);
    }

    function triggerAction(endpoint) {
      fetch(endpoint, { method: 'POST' })
        .then(r => r.text())
        .then(txt => showAlert(txt))
        .catch(err => showAlert('Error: ' + err));
    }

    document.getElementById('printForm').addEventListener('submit', function(e) {
      e.preventDefault();
      const formData = new FormData(this);
      fetch('/print', { method: 'POST', body: formData })
        .then(r => r.text())
        .then(txt => {
          showAlert(txt);
          document.getElementById('printText').value = '';
        })
        .catch(err => showAlert('Error: ' + err));
    });

    function updateStatus() {
      fetch('/status')
        .then(r => r.json())
        .then(data => {
          document.getElementById('badgeStatus').innerText = data.status;
          document.getElementById('badgeStatus').className = 'val ' + (data.status === 'READY' ? 'ok' : 'warn');
          
          document.getElementById('badgePaper').innerText = data.paper_out ? 'OUT OF PAPER' : 'LOADED';
          document.getElementById('badgePaper').className = 'val ' + (data.paper_out ? 'err' : 'ok');

          document.getElementById('badgeBuffer').innerText = data.buffer + ' B';
          document.getElementById('badgePrinted').innerText = data.printed + ' B';
          document.getElementById('ipPlaceholder').innerText = data.ip;
        })
        .catch(() => {});
    }

    setInterval(updateStatus, 1500);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";

static void handle_root() {
    server.send_P(200, "text/html", INDEX_HTML);
}

static void handle_status() {
    String json = "{";
    json += "\"status\":\"" + raw_server_get_printer_status() + "\",";
    json += "\"paper_out\":" + String(raw_server_is_paper_out() ? "true" : "false") + ",";
    json += "\"busy\":" + String(raw_server_is_busy() ? "true" : "false") + ",";
    json += "\"buffer\":" + String(raw_server_get_buffer_fill()) + ",";
    json += "\"printed\":" + String(raw_server_get_bytes_printed()) + ",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"flow_allowed\":" + String(raw_server_is_flow_allowed() ? "true" : "false");
    json += "}";
    server.send(200, "application/json", json);
}

static void handle_print() {
    String text = server.arg("text");
    if (text.length() == 0 && server.hasArg("plain")) {
        text = server.arg("plain");
    }

    if (text.length() == 0) {
        server.send(400, "text/plain", "No text provided");
        return;
    }

    // Optional formatting escape sequences
    bool nlq      = server.hasArg("nlq") && server.arg("nlq") == "1";
    bool bold     = server.hasArg("bold") && server.arg("bold") == "1";
    bool cond     = server.hasArg("condensed") && server.arg("condensed") == "1";
    bool elite    = server.hasArg("elite") && server.arg("elite") == "1";
    bool dwidth   = server.hasArg("dwidth") && server.arg("dwidth") == "1";
    bool eject    = server.hasArg("eject") && server.arg("eject") == "1";

    // Build byte stream
    // Ensure IBM Character Set #2 is selected: ESC m 2
    raw_server_push_byte(0x1B);
    raw_server_push_byte('m');
    raw_server_push_byte(2);

    if (nlq) {
        raw_server_push_byte(0x1B); raw_server_push_byte('x'); raw_server_push_byte(1); // ESC x 1: NLQ on
    }
    if (bold) {
        raw_server_push_byte(0x1B); raw_server_push_byte('E'); // ESC E: Bold on
    }
    if (cond) {
        raw_server_push_byte(0x0F); // SI: Condensed on (17 CPI)
    }
    if (elite) {
        raw_server_push_byte(0x1B); raw_server_push_byte('M'); // ESC M: Elite (12 CPI)
    }
    if (dwidth) {
        raw_server_push_byte(0x1B); raw_server_push_byte('W'); raw_server_push_byte(1); // ESC W 1: Double width
    }

    // Push payload text
    for (size_t i = 0; i < text.length(); i++) {
        raw_server_push_byte((uint8_t)text[i]);
    }
    raw_server_push_byte('\r');
    raw_server_push_byte('\n');

    // Reset styles back to normal defaults
    if (nlq)    { raw_server_push_byte(0x1B); raw_server_push_byte('x'); raw_server_push_byte(0); }
    if (bold)   { raw_server_push_byte(0x1B); raw_server_push_byte('F'); }
    if (cond)   { raw_server_push_byte(0x12); } // DC2: Cancel condensed
    if (elite)  { raw_server_push_byte(0x1B); raw_server_push_byte('P'); } // ESC P: Cancel elite (Pica)
    if (dwidth) { raw_server_push_byte(0x1B); raw_server_push_byte('W'); raw_server_push_byte(0); }

    if (eject) {
        raw_server_push_byte(0x0C); // FF (Form Feed)
    }

    server.send(200, "text/plain", "Job queued to printer successfully!");
}

static void handle_eject() {
    raw_server_push_byte(0x0C); // FF (Form feed)
    server.send(200, "text/plain", "Form feed dispatched");
}

static void handle_reset() {
    raw_server_send_command("!RESET");
    server.send(200, "text/plain", "Hardware reset signal sent to printer");
}

void web_portal_print_test_page() {
    // Escape sequences per Amstrad DMP3000 manual:
    // ESC @: Reset printer defaults
    // ESC m 2: Select IBM Character Set #2
    raw_server_push_byte(0x1B); raw_server_push_byte('@');
    raw_server_push_byte(0x1B); raw_server_push_byte('m'); raw_server_push_byte(2);

    String page = "";
    page += "======================================================================\r\n";
    page += "               AMSTRAD DMP3000 / IBM MODE TEST PAGE                   \r\n";
    page += "======================================================================\r\n\r\n";
    
    page += "System Status:\r\n";
    page += " - Network Host: " + String(MDNS_HOSTNAME) + ".local\r\n";
    page += " - IP Address:   " + WiFi.localIP().toString() + "\r\n";
    page += " - Port 9100:    RAW JetDirect / AppSocket (CUPS & macOS)\r\n";
    page += " - Port 515:     LPD Line Printer Daemon\r\n\r\n";

    page += "----------------------------------------------------------------------\r\n";
    page += "Typeface & Pitch Demonstrations:\r\n";
    page += " - Standard Pica (10 CPI, 80 columns per line)\r\n";

    // Stream first section
    for (size_t i = 0; i < page.length(); i++) raw_server_push_byte((uint8_t)page[i]);

    // Elite Mini (12 CPI): ESC M
    raw_server_push_byte(0x1B); raw_server_push_byte('M');
    String eliteStr = " - Elite Mini (12 CPI, 96 columns): ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789\r\n";
    for (size_t i = 0; i < eliteStr.length(); i++) raw_server_push_byte((uint8_t)eliteStr[i]);
    raw_server_push_byte(0x1B); raw_server_push_byte('P'); // Cancel Elite

    // Condensed (17 CPI): SI (0x0F)
    raw_server_push_byte(0x0F);
    String condStr = " - Condensed (17 CPI, 137 columns): The quick brown fox jumps over the lazy dog 1234567890\r\n";
    for (size_t i = 0; i < condStr.length(); i++) raw_server_push_byte((uint8_t)condStr[i]);
    raw_server_push_byte(0x12); // Cancel Condensed

    // Emphasized (Bold): ESC E
    raw_server_push_byte(0x1B); raw_server_push_byte('E');
    String boldStr = " - Emphasized Bold Print Style\r\n";
    for (size_t i = 0; i < boldStr.length(); i++) raw_server_push_byte((uint8_t)boldStr[i]);
    raw_server_push_byte(0x1B); raw_server_push_byte('F'); // Cancel Bold

    // Near Letter Quality (NLQ): ESC x 1
    raw_server_push_byte(0x1B); raw_server_push_byte('x'); raw_server_push_byte(1);
    String nlqStr = " - Near Letter Quality (NLQ, 26 CPS) High Density Text\r\n";
    for (size_t i = 0; i < nlqStr.length(); i++) raw_server_push_byte((uint8_t)nlqStr[i]);
    raw_server_push_byte(0x1B); raw_server_push_byte('x'); raw_server_push_byte(0); // Cancel NLQ

    // IBM CP437 Box Drawing and International Characters
    String box = "\r\n----------------------------------------------------------------------\r\n";
    box += "IBM Character Set #2 (CP437) Demonstration:\r\n";
    box += "+---------------------------------------------------+\r\n";
    box += "| [x] Box Drawing & Mathematical Characters         |\r\n";
    box += "| Spanish:  El veloz murcielago hindu comia feliz  |\r\n";
    box += "| Accents:  \xC1 \xC9 \xCD \xD3 \xDA \xE1 \xE9 \xED \xF3 \xFA \xF1 \xD1 \xBF \xA1 \xC7 \xE7          |\r\n";
    box += "| Currency: \xA3 $ \xA5                                      |\r\n";
    box += "+---------------------------------------------------+\r\n\r\n";
    box += "Self-test complete.\r\n";
    for (size_t i = 0; i < box.length(); i++) raw_server_push_byte((uint8_t)box[i]);

    // Eject page (Form Feed)
    raw_server_push_byte(0x0C);
}

static void handle_test_page() {
    web_portal_print_test_page();
    server.send(200, "text/plain", "Test page sent to printer");
}

void web_portal_init() {
    server.on("/", HTTP_GET, handle_root);
    server.on("/status", HTTP_GET, handle_status);
    server.on("/print", HTTP_POST, handle_print);
    server.on("/eject", HTTP_POST, handle_eject);
    server.on("/eject", HTTP_GET, handle_eject);
    server.on("/reset", HTTP_POST, handle_reset);
    server.on("/reset", HTTP_GET, handle_reset);
    server.on("/testpage", HTTP_POST, handle_test_page);
    server.on("/testpage", HTTP_GET, handle_test_page);

    server.begin();
}

void web_portal_loop() {
    server.handleClient();
}
