# Retro Wi-Fi Print Server for Amstrad DMP3000 (IBM Mode)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Platform: AVR & ESP8266](https://img.shields.io/badge/platform-AVR%20%7C%20ESP8266-blue.svg)]()
[![Protocol: CUPS / RAW 9100](https://img.shields.io/badge/protocol-CUPS%20%2F%20Port%209100-orange.svg)]()

Turn an iconic 1980s dot-matrix printer (**Amstrad DMP3000 / DMP3160 / DMP3250di**) into a modern, wireless network printer accessible from **Linux (CUPS)**, **macOS**, and **Android** using standard text-only printing protocols.

---

## 🛠️ Project Structure

This repository contains two coordinated Arduino projects and complete documentation:

```
oldprinter/
├── arduino_printer_driver/       # Project 1: Arduino Uno / Nano parallel interface firmware
│   ├── arduino_printer_driver.ino# Main loop, buffer management, status commands
│   ├── centronics.h / .cpp       # 8-bit parallel bus driver & Centronics handshaking
│   ├── cp437_transcode.h / .cpp  # UTF-8 streaming decoder -> IBM CP437 character mapper
│   └── config.h                  # Pin mapping, timing constants, flow control settings
├── esp8266_cups_bridge/          # Project 2: ESP8266 (ESP-01 8-pin) Wi-Fi network bridge
│   ├── esp8266_cups_bridge.ino   # Main setup, Wi-Fi station/AP, mDNS advertising
│   ├── cups_raw_server.h / .cpp  # Port 9100 RAW JetDirect & Port 515 LPD TCP servers
│   ├── web_portal.h / .cpp       # Embedded Web UI (Port 80) for direct mobile/desktop printing
│   └── config.h                  # Wi-Fi credentials, network ports, baud rate
└── docs/
    ├── HARDWARE_WIRING_AND_CUPS_SETUP.md # Complete pinouts, voltage dividers, CUPS/Mac/Android guides
    └── Amstrad_DMP3000,D3160,DMP3250di_User_Manual.md # Full technical manual & command codes
```

---

## 💡 System Architecture

```
[ macOS / Linux / Android ]
        │
        │ Wi-Fi (Raw TCP Port 9100 / LPD Port 515 / Web Port 80)
        ▼
[ ESP8266 (ESP-01 8-Pin) ]
        │
        │ 115200 Baud UART with XON/XOFF Flow Control
        │ (Arduino 5V TX stepped down to 3.3V with 1kΩ / 2kΩ divider)
        ▼
[ Arduino Uno / Nano (5V ATmega328P) ]
        │
        │ 8-Bit Parallel Centronics Bus (DB25 Connector)
        ▼
[ Amstrad DMP3000 Dot-Matrix Printer ] (IBM Character Set #2)
```

---

## ⚡ Hardware Features

1. **Rock-Solid Centronics Handshake**: Meets exact Amstrad DMP3000 timing specifications ($\ge 0.5\,\mu\text{s}$ data setup, $\ge 0.5\,\mu\text{s}$ `/STROBE` pulse, polling `BUSY`, with `PE` paper-out and `/FAULT` error handling).
2. **End-to-End Backpressure Pipeline**: Dot-matrix printing is slow (105 CPS draft, 26 CPS NLQ, 200 ms line feeds). The Arduino throttles the ESP8266 using `XOFF` (`0x13`) and `XON` (`0x11`). When the ESP buffer fills, it closes the TCP receive window, throttling the CUPS spooler upstream so multi-page documents never drop characters.
3. **UTF-8 to IBM CP437 Transcoding**: Translates accented characters (`á`, `é`, `ñ`, `ü`, `ç`, `¿`, `¡`) and box-drawing symbols into native **IBM Character Set #2** glyphs on the fly, preventing garbled multi-byte characters.
4. **Anti-Staircase Newline Filter**: Automatically injects Carriage Return (`\r`) before Line Feed (`\n`) if missing from UNIX/macOS print streams.

---

## 🔌 Hardware Wiring Summary

### 1. DB25 to Arduino Uno / Nano

| DB25 Pin | Centronics 36-Pin | Signal | Arduino Pin |
| :---: | :---: | :--- | :---: |
| **1** | 1 | `/STROBE` | **D10** |
| **2 – 9**| 2 – 9 | DATA 0 – 7 | **D2 – D9** |
| **10** | 10 | `/ACK` | **D12** |
| **11** | 11 | BUSY | **D11** |
| **12** | 12 | PE (Paper End) | **A2** |
| **13** | 13 | SELECT | **A3** |
| **15** | 32 | `/FAULT` | **A1** |
| **16** | 31 | `/INIT` | **A0** |
| **17** | 36 | `/SLCT_IN` | **GND** |
| **18 – 25**| 19 – 30 | GND | **GND** |

### 2. Arduino to ESP8266 (ESP-01 8-Pin)

| ESP-01 Pin | Signal | Connection |
| :---: | :--- | :--- |
| **1** | GND | Arduino **GND** & External 3.3V PSU **GND** |
| **2** | TX (GPIO1) | Arduino **Pin 0 (RX)** (Direct 3.3V $\to$ 5V) |
| **3** | GPIO2 | Pull-up to **3.3V** via 10 kΩ resistor |
| **4** | CH_PD (EN) | Connect to **+3.3V** |
| **5** | GPIO0 | Pull-up to **3.3V** via 10 kΩ resistor (pull LOW to flash) |
| **6** | RST | Connect to **+3.3V** |
| **7** | RX (GPIO3) | Arduino **Pin 1 (TX)** via **1 kΩ / 2 kΩ Voltage Divider** |
| **8** | VCC | **Dedicated 3.3V Power Supply** (AMS1117-3.3 + 100 µF cap) |

> [!CAUTION]
> **Power Supply Warning:** Do NOT power the ESP-01 from the Arduino Nano's onboard 3.3V pin. The ESP-01 requires up to 300 mA during RF transmission bursts, which will brown out the Nano's internal 3.3V regulator. Use a dedicated 3.3V regulator and place a 100 µF capacitor across ESP VCC and GND.

---

## 🖨️ Printer DIP Switch Settings

On the rear of the Amstrad DMP3000, configure the switches to select **IBM Character Set #2**:
- **Bank DS1:** DS1-1 to DS1-3: **ON** (USA), DS1-4: **OFF** (CR only), DS1-5: **OFF** (Paper sensor enabled), DS1-7: **ON**, DS1-8: **ON** (IBM Character Set #2).
- **Bank DS2:** DS2-5: **ON** (`/SLCT_IN` internally asserted), all others **OFF**.

---

## 🚀 Quick Start & Compilation

### Using `arduino-cli`:

```bash
# 1. Compile Arduino Uno / Nano driver
arduino-cli compile --fqbn arduino:avr:uno arduino_printer_driver
# (Or for Nano: arduino-cli compile --fqbn arduino:avr:nano arduino_printer_driver)

# 2. Upload to Arduino
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno arduino_printer_driver

# 3. Compile and upload ESP8266 firmware:
arduino-cli compile --fqbn esp8266:esp8266:generic esp8266_cups_bridge
arduino-cli upload -p /dev/ttyUSB1 --fqbn esp8266:esp8266:generic esp8266_cups_bridge
```

---

## 📱 Direct Wi-Fi Printing (Zero Router Required!)

The ESP8266 generates its own standalone Wi-Fi Access Point containing its unique hardware Chip UID:
* **Network SSID:** `Amstrad-DMP3000-<UID>` (e.g. `Amstrad-DMP3000-8C4E2A`)
* **Password:** *(None / Open by default — connect instantly!)*
* **Printer Direct IP:** `192.168.4.1`

### 1. From macOS
1. Connect your Mac's Wi-Fi directly to `Amstrad-DMP3000-<UID>`.
2. Open **System Settings > Printers & Scanners > Add Printer (+)**.
3. Click the **IP** tab.
4. Address: `192.168.4.1` (or `oldprinter.local`).
5. Protocol: **HP Jetdirect - Socket** (port 9100).
6. Use: **Generic Text-Only Printer** or **IBM Proprinter**.
7. Print any file: `lp -d Amstrad_DMP3000 file.txt`.

### 2. From Linux (CUPS)
1. Connect Wi-Fi to `Amstrad-DMP3000-<UID>`.
2. Add printer to CUPS:
```bash
sudo lpadmin -p Amstrad_DMP3000 -v socket://192.168.4.1:9100 -E -m raw
lp -d Amstrad_DMP3000 document.txt
```

### 3. From Android
1. Connect phone Wi-Fi to `Amstrad-DMP3000-<UID>`.
2. **Zero-Install Web Print:** Open `http://192.168.4.1` in Chrome. Type or paste your note, select font styles (NLQ, Bold, Condensed), and tap **Print Text Now**!
3. **Raw Socket Apps:** Use apps like **RawBT** or **PrintBot** pointing to `192.168.4.1:9100`.

---

## 📖 Detailed Guides

For full schematics, LPD protocols, PPD configurations, and font code listings, see:
- [Hardware Wiring & CUPS Setup Guide](file:///home/spere4/Documents/oldprinter/docs/HARDWARE_WIRING_AND_CUPS_SETUP.md)
- [Amstrad DMP3000 User Manual & Reference](file:///home/spere4/Documents/oldprinter/docs/Amstrad_DMP3000,D3160,DMP3250di_User_Manual.md)
