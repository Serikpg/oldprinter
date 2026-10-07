# Hardware Wiring, Interface Specifications & CUPS Setup Guide

This guide provides the complete hardware wiring diagrams, electrical interface specifications, DIP switch configurations, and network printing setup (CUPS on Linux, macOS, and Android) for connecting an **Amstrad DMP3000 / DMP3160 / DMP3250di** dot-matrix printer to an **Arduino Uno / Nano** bridged to an **ESP8266 (ESP-01 8-pin)** Wi-Fi controller.

---

## 1. System Architecture Overview

```
                      +-----------------------------+
                      | macOS / Linux (CUPS)        |
                      | Android (Web / Raw Socket)  |
                      +--------------+--------------+
                                     |
                                     | Wi-Fi (Port 9100 RAW / Port 515 LPD / Port 80 Web)
                                     v
                       +---------------------------+
                       |    ESP8266 (ESP-01)       |
                       |    Wi-Fi Print Server     |
                       +-------------+-------------+
                                     |
                                     | Hardware UART (115200 Baud)
                                     | [5V <-> 3.3V Level Shifter]
                                     | Software XON/XOFF Flow Control
                                     v
                       +---------------------------+
                       |    Arduino Uno / Nano     |
                       | Parallel Centronics Driver|
                       |  UTF-8 -> CP437 Decoder   |
                       +-------------+-------------+
                                     |
                                     | 8-Bit Parallel Bus + Centronics Handshake
                                     | DB25 Male Connector
                                     v
                       +---------------------------+
                       |  Amstrad DMP3000 Printer  |
                       |  (IBM Character Set #2)   |
                       +---------------------------+
```

---

## 2. Printer Interface & DB25 Wiring

The printer chassis has an industry-standard **Amphenol 36-pin female Centronics** socket. The cable standard converts this to an **IBM PC DB25 male** plug.

### DB25 to Arduino Uno / Nano Pinout Table

All logic levels between the DB25 parallel port and the Arduino ATmega328P operate at **5V TTL**.

| DB25 Pin | Centronics 36-Pin | Signal Designation | Direction (Rel. to Arduino) | Arduino Uno / Nano Pin | Function & Description |
| :---: | :---: | :--- | :---: | :---: | :--- |
| **1** | 1 | `/STROBE` | **OUT** | **D10** | Active LOW pulse ($\ge 0.5\,\mu\text{s}$) to latch data into printer. |
| **2** | 2 | DATA 0 (LSB) | **OUT** | **D2** | Parallel data bit 0. |
| **3** | 3 | DATA 1 | **OUT** | **D3** | Parallel data bit 1. |
| **4** | 4 | DATA 2 | **OUT** | **D4** | Parallel data bit 2. |
| **5** | 5 | DATA 3 | **OUT** | **D5** | Parallel data bit 3. |
| **6** | 6 | DATA 4 | **OUT** | **D6** | Parallel data bit 4. |
| **7** | 7 | DATA 5 | **OUT** | **D7** | Parallel data bit 5. |
| **8** | 8 | DATA 6 | **OUT** | **D8** | Parallel data bit 6. |
| **9** | 9 | DATA 7 (MSB) | **OUT** | **D9** | Parallel data bit 7. |
| **10** | 10 | `/ACK` | **IN** | **D12** | Active LOW pulse ($\approx 5\,\mu\text{s}$) when byte is processed. |
| **11** | 11 | BUSY | **IN** | **D11** | HIGH when printer buffer full, printing, or offline. |
| **12** | 12 | PE (Paper End) | **IN** | **A2 (D16)** | HIGH when printer is out of paper. |
| **13** | 13 | SELECT | **IN** | **A3 (D17)** | HIGH when printer is online. |
| **14** | 14 | `/AUTOFD` | — | *NC or GND* | Leave unconnected (software handles CR/LF). |
| **15** | 32 | `/FAULT` / `/ERROR` | **IN** | **A1 (D15)** | Active LOW on printer error or offline state. |
| **16** | 31 | `/INIT` | **OUT** | **A0 (D14)** | Active LOW pulse ($\ge 100\,\mu\text{s}$) to reset printer. |
| **17** | 36 | `/SLCT_IN` | — | *GND* | Connect to GND to select printer (or float if DS2-5 is ON). |
| **18–25** | 19–30 | GND | — | **GND** | Signal and logic ground reference (connect all together). |

> [!NOTE]
> DB25 pins 18 through 25 are ground pins. Wire at least one (ideally two or more) directly to the Arduino's `GND` pin to maintain clean signal references and eliminate ground loops.

---

## 3. Arduino Uno / Nano to ESP8266 (ESP-01) Bridge

The **ESP-01** is a compact 8-pin module with an ESP8266 chip.

### ESP-01 8-Pin Header Pinout

```
           +-------------+
       GND | [1]     [2] | TX (GPIO1)
     GPIO2 | [3]     [4] | CH_PD (EN)
     GPIO0 | [5]     [6] | RST
RX (GPIO3) | [7]     [8] | VCC (+3.3V ONLY)
           +-----+ +-----+
                 | |
             [Antenna]
```

### Voltage Level Translation & Wiring

- **ESP-01 TX (3.3V) $\to$ Arduino RX (Pin 0, 5V):**  
  The ATmega328P input high threshold is $V_{IH} = 0.6 \times 5\,\text{V} = 3.0\,\text{V}$. The ESP8266's $3.3\,\text{V}$ output exceeds $3.0\,\text{V}$ and can be connected directly to Arduino Pin 0.
- **Arduino TX (Pin 1, 5V) $\to$ ESP-01 RX (Pin 7, 3.3V):**  
  **DO NOT connect directly without level shifting!** The ESP8266 is a 3.3V device. Use a simple 2-resistor voltage divider or a MOSFET level shifter module.

#### Resistor Voltage Divider Schematic

```
Arduino TX (Pin 1, 5V)
        |
      [1 kΩ]
        |
        +-----> ESP-01 RX (Pin 7, 3.3V)
        |
      [2 kΩ] (or 2.2 kΩ)
        |
       GND
```

Calculation: $V_{\text{RX}} = 5\,\text{V} \times \frac{2\,\text{k}\Omega}{1\,\text{k}\Omega + 2\,\text{k}\Omega} \approx 3.33\,\text{V}$.

### Pin Connection Table

| ESP-01 Pin | Name | Connection | Notes |
| :---: | :--- | :--- | :--- |
| **1** | GND | Arduino **GND** & Power Supply **GND** | Common ground reference. |
| **2** | TX (GPIO1) | Arduino **Pin 0 (RX)** | Direct connection (3.3V output to 5V input). |
| **3** | GPIO2 | Pull-up via **10 kΩ resistor to 3.3V** | Must be HIGH during boot. |
| **4** | CH_PD / EN | Connect to **+3.3V** (or via 10 kΩ pull-up) | Chip enable; must be pulled HIGH to operate. |
| **5** | GPIO0 | Pull-up via **10 kΩ resistor to 3.3V** | Must be HIGH for normal flash run mode. |
| **6** | RST | Connect to **+3.3V** (or via 10 kΩ pull-up) | Active LOW hardware reset. |
| **7** | RX (GPIO3) | From Arduino **Pin 1 (TX)** via **Voltage Divider** | Stepped down from 5V to 3.3V. |
| **8** | VCC | **+3.3V Dedicated Power Supply** | **See power supply warning below!** |

### ⚠️ Powering the ESP-01: 5V vs 3.3V and Regulators vs Voltage Dividers

#### 1. Why Can't You Use the Arduino's Onboard 3.3V Pin (Even on USB)?
Even if your Arduino Uno or Nano is plugged into a high-power USB port or wall charger, **the 3.3V pin cannot power an ESP8266**:
- **On Arduino Nano:** There is no dedicated 3.3V regulator on the board! The `3V3` pin is fed from the internal reference LDO inside the USB-serial converter chip (FTDI FT232RL or CH340G).
  - FT232RL pin 17 max current: **50 mA**
  - CH340G pin 4 max current: **25–30 mA**
- **On Arduino Uno:** The onboard LP2985 3.3V LDO is rated for a maximum of **50 mA** (150 mA on some revisions).
- **ESP8266 Power Demand:** In idle listening mode, the ESP draws ~70 mA. When calibrating its radio or transmitting Wi-Fi packets, current spikes reach **170 mA to 280 mA** (with microsecond bursts over 300 mA).
- **Consequence:** Connecting the ESP-01 to the Arduino 3.3V pin causes the voltage to instantly collapse from 3.3V down to ~2.2V. The ESP triggers a hardware brownout reset, entering an endless reboot loop (`rst cause:2, boot mode:(3,7)`). On a Nano, it can also permanently damage the CH340/FTDI chip.

---

#### 2. Can You Use a Voltage Divider for Power (VCC)?
**NO! Never use a voltage divider to power an active circuit or microchip.**
- A resistive voltage divider ($R_1$ and $R_2$) only works for **signals** (like logic pins with negligible current draw, $< 1\,\mu\text{A}$).
- When used for power, the load (the ESP8266) is effectively in parallel with $R_2$. As the ESP's current draw fluctuates between 15 mA and 280 mA, its effective resistance changes wildly.
- To keep the voltage stable under a 200 mA load, the divider resistors would have to be tiny (e.g. $10\,\Omega$ and $20\,\Omega$), which would continuously burn $> 160\,\text{mA}$ as pure heat ($> 0.8\,\text{W}$) and still drop down to 1.5V during RF transmit bursts.

---

#### 3. Can You Use the Arduino 5V Pin + a Voltage Regulator?
**YES! This is the standard, reliable method.**
The Arduino `5V` pin (when powered from USB) is connected directly to the USB power rail through a 500 mA polyfuse. Since the ATmega328P consumes only ~20 mA, you have over **400 mA of clean 5V current available** on the `5V` pin.

You can step this 5V down to 3.3V using any low-cost linear regulator or dedicated adapter:

```
Arduino 5V Pin ──────────────> [ VIN ]
                               AMS1117-3.3 ──> [ VOUT (3.3V) ] ───> ESP-01 Pin 8 (VCC)
Arduino GND Pin ─────────────> [ GND ]                         ───> ESP-01 Pin 1 (GND)
                                                 │
                                              [100 µF] (Electrolytic across 3.3V & GND)
```

**Recommended Options:**
1. **AMS1117-3.3 Module / Chip:** A tiny 3-pin LDO regulator module ($0.50) that accepts 5V input and delivers up to 800 mA at 3.3V.
2. **ESP-01 Breadboard Adapter Board:** A $1 pre-made socket board with a built-in AMS1117-3.3 regulator and decoupling capacitor. You simply plug the ESP-01 into the socket and power it directly from Arduino 5V and GND.
3. **Emergency Bench Trick (Two Diodes in Series):** If you don't have an AMS1117 on hand, two standard silicon diodes (e.g., 1N4001 or 1N4007) in series drop $2 \times 0.7\,\text{V} = 1.4\,\text{V}$, giving $5.0\,\text{V} - 1.4\,\text{V} = 3.6\,\text{V}$ (the absolute max for ESP8266). Combined with a 100 µF capacitor, this will work for bench testing.

---

## 4. Amstrad DMP3000 DIP Switch Configuration

The printer contains two DIP switch banks accessible on the rear:
- **Bank DS1:** 8 switches
- **Bank DS2:** 10 switches

To configure the printer for standard IBM Mode (Table 3.2 — IBM Character Set #2 with extended symbols and box drawing), set the switches as follows:

> [!IMPORTANT]
> Always turn the printer **POWER OFF** before toggling any DIP switches.

### Bank DS1 (8 Switches)

| Switch | Setting | Function |
| :---: | :---: | :--- |
| **DS1-1** | **ON** | USA character set |
| **DS1-2** | **ON** | USA character set |
| **DS1-3** | **ON** | USA character set |
| **DS1-4** | **OFF** | `CR` only (Arduino firmware handles LF/CR translation to prevent staircase effect) |
| **DS1-5** | **OFF** | Paper-out sensor enabled (halts printing when out of paper) |
| **DS1-6** | **OFF** | Page length 11 inches (standard fanfold paper) |
| **DS1-7** | **ON** | Character set select: IBM Set |
| **DS1-8** | **ON** | Character set select: **IBM Character Set #2** (Factory Default) |

### Bank DS2 (10 Switches)

| Switch | Setting | Function |
| :---: | :---: | :--- |
| **DS2-1** | **OFF** | Unslashed zero ($0$) |
| **DS2-2** | **OFF** | Skip perforation disabled |
| **DS2-3** | **OFF** | Standard character buffer |
| **DS2-4** | **OFF** | Standard buffer allocation |
| **DS2-5** | **ON** | `/SLCT_IN` automatically asserted internally |
| **DS2-6** | **ON** | Alarm buzzer enabled |
| **DS2-7** | **OFF** | Power-on standard typeface (Bold OFF) |
| **DS2-8** | **OFF** | Power-on standard typeface (Condensed OFF) |
| **DS2-9** | **OFF** | Factory reserved |
| **DS2-10**| **OFF** | Factory reserved |

---

## 5. Network Printing Configuration (CUPS, macOS & Android)

The ESP8266 exposes three network services:
1. **Port 9100 (RAW AppSocket / HP JetDirect):** The universal raw text stream protocol used by CUPS, macOS, Windows, and raw print services.
2. **Port 515 (LPD / LPR):** Line Printer Daemon (RFC 1179).
3. **Port 80 (HTTP Web Portal):** Browser dashboard with direct text printing for mobile devices.

### A. CUPS on Linux

#### Method 1: Using the Command Line (`lpadmin`)

Open a terminal on your Linux system:

```bash
# Add printer using RAW AppSocket backend with Generic Text-Only driver
sudo lpadmin -p Amstrad_DMP3000 \
             -v socket://oldprinter.local:9100 \
             -E \
             -m raw \
             -D "Amstrad DMP3000 Dot Matrix" \
             -L "Workbench"

# Alternatively, using the CUPS textonly driver with PPD:
sudo lpadmin -p Amstrad_DMP3000 \
             -v socket://oldprinter.local:9100 \
             -E \
             -m textonly.ppd \
             -D "Amstrad DMP3000 Dot Matrix"

# Set as default printer (optional)
sudo lpoptions -d Amstrad_DMP3000
```

#### Method 2: Using the CUPS Web Interface

1. Open your browser and navigate to `http://localhost:631/admin`.
2. Click **Add Printer** (enter your Linux user/password if prompted).
3. Under *Other Network Printers*, select **AppSocket/HP JetDirect** (or select the automatically discovered Bonjour printer `oldprinter`).
4. Enter the Connection URI:
   ```
   socket://<ESP_IP_ADDRESS>:9100
   ```
   *(or `socket://oldprinter.local:9100`)*
5. Enter Name: `Amstrad_DMP3000`.
6. When selecting Make/Model:
   - Select **Generic** $\to$ **Generic Text-Only Printer (en)**, or
   - Select **IBM** $\to$ **IBM Proprinter (en)**.
7. Click **Add Printer** $\to$ Set Default Options.

#### Test Print from Linux:

```bash
# Print a plain text file:
lp -d Amstrad_DMP3000 document.txt

# Or stream raw text directly using netcat:
echo -e "Hello Amstrad DMP3000\r\nPrinted from Linux CUPS!\f" | nc oldprinter.local 9100
```

---

### B. macOS Setup

macOS uses CUPS natively under the hood and supports AppSocket / JetDirect and LPD protocols.

1. Open **System Settings** (or System Preferences).
2. Navigate to **Printers & Scanners**.
3. Click **Add Printer, Scanner, or Fax...** (the `+` button).
4. Click the **IP** tab (globe icon) at the top of the window.
5. Fill in the fields:
   - **Address:** `oldprinter.local` (or the IP address assigned to the ESP8266, e.g. `192.168.1.150`).
   - **Protocol:** Select **HP Jetdirect - Socket** (port 9100) or **Line Printer Daemon - LPD** (port 515).
   - **Queue:** Leave blank (for Socket) or enter `raw` (for LPD).
   - **Name:** `Amstrad DMP3000`
   - **Location:** `Retro Workstation`
   - **Use:** Click the dropdown, choose **Select Software...**, search for **Generic Text-Only Printer** or **IBM Proprinter**, and select it.
6. Click **Add**.

#### Printing from macOS Terminal:

```bash
# Print any text file
lp -d Amstrad_DMP3000 my_code.py

# Send formatted banner directly
banner "HELLO MAC" | lp -d Amstrad_DMP3000
```

---

### C. Android Setup

There are three convenient ways to print from Android devices:

#### Option 1: Zero-Install Direct Web Print (Recommended for Phones & Tablets)
No drivers or applications are needed:
1. Connect your Android phone to the same Wi-Fi network.
2. Open Chrome (or any browser) and navigate to `http://oldprinter.local` or `http://<ESP_IP>`.
3. The responsive retro **Amstrad DMP3000 Print Server** dashboard opens.
4. Type or paste your message or notes into the text box.
5. Select formatting options if desired (*Near Letter Quality*, *Bold*, *Condensed*, *Form Feed*).
6. Tap **Print Text Now**!

#### Option 2: Android Raw Socket Print Apps
To print from Android's native share menu or apps:
1. Install an app supporting raw JetDirect printing such as **RawBT**, **PrintBot**, or **CUPS Print** from Google Play.
2. In the app settings:
   - Printer type: **Network (WiFi / Ethernet)**
   - Protocol: **RAW / TCP / AppSocket**
   - IP Address: `<ESP_IP_ADDRESS>`
   - Port: `9100`
   - Character encoding: **IBM CP437** or **UTF-8** (the Arduino firmware will automatically decode UTF-8 accents).
3. Now you can use Android's "Share" or "Print" feature to send text directly to the printer.

#### Option 3: Shared via CUPS AirPrint Server
If you run CUPS on a Raspberry Pi or home server on your network with Avahi/AirPrint enabled (`cups-browsed` or DNS-SD AirPrint TXT records), Android devices with the *Default Print Service* will automatically discover the Amstrad DMP3000 in the system print dialog.

---

## 6. Verification and Troubleshooting

| Symptom | Probable Cause | Corrective Action |
| :--- | :--- | :--- |
| **ESP-01 constantly restarts / reboots** | Insufficient 3.3V power during Wi-Fi transmit bursts | Do not power ESP-01 from Arduino 3.3V pin. Use external 3.3V regulator (AMS1117) and add a 100 µF capacitor across ESP VCC/GND. |
| **Garbled characters printed** | Baud rate mismatch or floating ground | Verify baud rate is set to `115200` on both sketches. Ensure DB25 ground pins (18–25) and Arduino GND are firmly connected. |
| **Text prints in steps / stairs** | Newlines lack carriage returns (`\n` without `\r`) | `AUTO_CR_ON_LF` is enabled by default in `config.h`. Ensure DIP switch `DS1-4` is OFF. |
| **Accented letters (é, á, ñ, ü) garbled** | UTF-8 multi-byte characters not converted | The included `cp437_transcode` engine converts UTF-8 to IBM CP437 automatically. Ensure DIP switches `DS1-7` and `DS1-8` are ON (IBM Set #2). |
| **Printer status shows "PAPER OUT"** | PE line disconnected or paper sensor triggered | Ensure paper is correctly loaded into tractor/friction platen. Check wiring of DB25 Pin 12 to Arduino Pin A2. |
| **Arduino stops receiving data mid-page** | Serial buffer overrun during slow printing | The firmware uses XON/XOFF flow control. Ensure Arduino TX (Pin 1) is wired to ESP-01 RX (Pin 7) through the voltage divider. |
