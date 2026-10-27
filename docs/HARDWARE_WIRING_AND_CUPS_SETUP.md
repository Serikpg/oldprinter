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
| **5** | 5 | DATA 3 | **OUT** | **D5** | Parallel data bit 3. *(Verify: do not swap with D6!)* |
| **6** | 6 | DATA 4 | **OUT** | **D6** | Parallel data bit 4. *(Verify: do not swap with D5!)* |
| **7** | 7 | DATA 5 | **OUT** | **D7** | Parallel data bit 5. |
| **8** | 8 | DATA 6 | **OUT** | **D8** | Parallel data bit 6. |
| **9** | 9 | DATA 7 (MSB) | **OUT** | **D9** | Parallel data bit 7. |
| **10** | 10 | `/ACK` | **IN** | **D12** | Active LOW pulse ($\approx 5\,\mu\text{s}$) when byte is processed. |
| **11** | 11 | BUSY | **IN** | **D11** | HIGH when printer buffer full, printing, or offline. Primary flow control. |
| **12** | 12 | PE (Paper End) | **IN** | **A2 (D16)** | HIGH when printer is out of paper. |
| **13** | 13 | SELECT | **IN** | **A3 (D17)** | HIGH when printer is online. |
| **14** | 14 | `/AUTOFD` | — | *NC or GND* | Leave unconnected (software handles CR/LF). |
| **15** | 32 | `/FAULT` / `/ERROR` | **IN** | *Leave Disconnected* (A1) | Ignored in software (`IGNORE_FAULT_PIN true`) to prevent false freezes. |
| **16** | 31 | `/INIT` | **OUT** | **A0 (D14)** | Active LOW pulse ($\ge 100\,\mu\text{s}$) to reset printer on boot. |
| **17** | 36 | `/SLCT_IN` | — | **GND** | Connect to GND to force printer selected (or toggle DIP switch DS2-5 ON). |
| **18–25** | 19–30 | GND | — | **GND** | Signal and logic ground reference (connect at least Pin 18 to GND). |

> [!TIP]
> **Data Bit 3 vs Bit 4 Wiring Trap:**
> Double check that **DB25 Pin 5 connects to Arduino D5** and **DB25 Pin 6 connects to Arduino D6**. If these two wires are transposed, bit 3 and bit 4 get swapped in every ASCII byte:
> - `'H'` (`0x48`) becomes `'P'` (`0x50`)
> - `'l'` (`0x6C`) becomes `'t'` (`0x74`)
> - `'o'` (`0x6F`) becomes `'w'` (`0x77`)
> - Turning `Hello` into `Pettw` and `Heartbeat` into `Peajlbeal`!

> [!NOTE]
> **Fault Pin Disconnection:**
> In vintage Centronics cables and printers, Pin 15 (`/FAULT`) is often grounded or asserted when the printer is offline or when using cut-sheet paper. The Arduino firmware configures `IGNORE_FAULT_PIN = true` by default and uses `BUSY` and `PE` for hardware control. **Leave Arduino pin A1 disconnected** to allow uninterrupted printing and page ejects.

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

You can step this 5V down to 3.3V using any low-cost linear regulator or dedicated buck converter:

```
Arduino 5V Pin ──────────────> [ VIN ]
                               AMS1117-3.3 ──> [ VOUT (3.3V) ] ───> ESP-01 Pin 8 (VCC)
Arduino GND Pin ─────────────> [ GND ]                         ───> ESP-01 Pin 1 (GND)
                                                 │
                                           [100 µF – 470 µF] (Electrolytic across 3.3V & GND)
```

> [!IMPORTANT]
> **Mandatory Decoupling Capacitor (100 µF to 470 µF):**
> You **MUST** solder or plug an electrolytic capacitor (100 µF to 470 µF, rated 6.3V or higher) directly across the ESP-01 **`VCC`** and **`GND`** pins.
> 
> **Why is this critical?**
> During boot, the ESP8266 initializes its physical radio layer (`phy_init`) and runs RF transmitter power calibration (`ram_rfcal_pwrctrl` / `ram_rfcal_txiq`). At that exact microsecond, the RF power amplifier turns on at +20 dBm and draws an instantaneous **350–400 mA current pulse**.
> 
> Without a local reservoir capacitor:
> - Even with a 2A-rated buck converter or regulator, the feedback loop cannot react fast enough. The 3.3V rail sags below ~2.7V for a few microseconds.
> - A standard multimeter will falsely report "stable 3.4V" because it only measures average DC voltage.
> - Symptoms of missing capacitor:
>   - RF scanner reports `Detected 0 networks` even in rooms with multiple Wi-Fi routers.
>   - The Access Point beacon never appears on laptops/smartphones.
>   - Serial console outputs: `Fatal exception 0 (IllegalInstructionCause): epc1=0x40242f63 in ram_rfcal_txiq`.
> - Adding a **200 µF – 470 µF capacitor** right across `VCC` and `GND` completely eliminates this sag and guarantees 100% RF transmit stability.

**Recommended Regulator Options:**
1. **AMS1117-3.3 Module / Chip:** A tiny 3-pin LDO regulator module ($0.50) that accepts 5V input and delivers up to 800 mA at 3.3V.
2. **Step-Down Buck Converter (e.g. MP1584 / Mini-360):** Very efficient. Note that buck regulators require a 1.5V+ dropout headroom; ensure input is at least 5V and keep the 200 µF decoupling capacitor on the 3.3V output rail.
3. **ESP-01 Breadboard Adapter Board:** A $1 pre-made socket board with a built-in AMS1117-3.3 regulator and decoupling capacitor.

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
| **DS1-4** | **OFF** | **`CR` only (Single line spacing)**. *Crucial:* If set to ON, the printer performs an Auto-LF on every CR, resulting in an unwanted empty blank line (double spacing) between rows! |
| **DS1-5** | **OFF** | Paper-out sensor enabled (halts printing when out of paper) |
| **DS1-6** | **OFF** | Page length 11 inches (standard fanfold paper) |
| **DS1-7** | **ON** | Character set select: IBM Set |
| **DS1-8** | **ON** | Character set select: **IBM Character Set #2** (Factory Default) |

> [!TIP]
> **Line Spacing Troubleshooting:**
> - If your prints have an **empty line between lines of text** (double spacing), check **DS1-4**: it must be toggled **OFF**. With DS1-4 OFF, standard single spacing (6 Lines Per Inch) is achieved.
> - For dense/compact vertical line spacing, the DMP3000 also supports **8 Lines Per Inch (1/8" pitch)** via the escape sequence `ESC 0` (`\x1B0`).

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

## 5. Direct Standalone Network Printing (Zero Router Required!)

The ESP8266 generates its own independent Wi-Fi network directly:
- **SSID:** `Amstrad-DMP3000-<UID>` (where `<UID>` is the unique 6-character hardware Chip ID of the ESP8266, e.g. `Amstrad-DMP3000-8C4E2A`).
- **Security:** Open by default (no password required for instant connection; configurable in `config.h`).
- **Printer Direct IP:** `192.168.4.1` (built-in DHCP automatically assigns an IP to your connected device).

The ESP8266 exposes three direct network services on `192.168.4.1`:
1. **Port 9100 (RAW AppSocket / HP JetDirect):** The universal raw text stream protocol used by CUPS, macOS, and Android raw print services (`socket://192.168.4.1:9100`).
2. **Port 515 (LPD / LPR):** Line Printer Daemon (`lpd://192.168.4.1/raw`).
3. **Port 80 (HTTP Web Portal):** Browser dashboard with direct text printing (`http://192.168.4.1/`).
4. **Port 53 (Captive DNS):** Automatically resolves any URL entered on a connected phone or laptop directly to `192.168.4.1`.

---

### A. CUPS on Linux

1. Connect your computer's Wi-Fi directly to `Amstrad-DMP3000-<UID>`.
2. Add the printer using the command line (`lpadmin`):

```bash
# Add printer using RAW AppSocket backend with Generic Text-Only driver
sudo lpadmin -p Amstrad_DMP3000 \
             -v socket://192.168.4.1:9100 \
             -E \
             -m raw \
             -D "Amstrad DMP3000 Dot Matrix" \
             -L "Workbench"

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
| **`Hello` prints as `Pettw` (or scrambled letters)** | **Bits 3 and 4 are swapped** | Swap Arduino **Pin D5** and **Pin D6**. (Verify: DB25 Pin 5 $\to$ Arduino D5; DB25 Pin 6 $\to$ Arduino D6). |
| **Double spacing (empty blank line between rows)** | Printer has Auto-LF enabled on CR | Power off printer, toggle rear DIP switch **DS1-4 to OFF** (`CR only`), then power on. |
| **Printer halts / refuses to eject (`fault = 1`)** | False fault signal on DB25 Pin 15 | Disconnect wire from Arduino **Pin A1**. The driver software configures `IGNORE_FAULT_PIN true` and relies on `BUSY` and `PE`. |
| **Wi-Fi scan detects 0 networks or brownout crash** | Missing/insufficient reservoir capacitor | Solder or connect an electrolytic capacitor (**200 µF to 470 µF**) directly across ESP-01 **`VCC`** and **`GND`** pins to buffer the 350 mA RF calibration pulse. |
| **Phone cannot open `192.168.4.1` or drops connection** | Mobile OS auto-disconnects from AP without Internet | In your phone's Wi-Fi settings, tap `Amstrad-DMP3000-<UID>`, and when prompted that the network has no Internet, tap **"Stay Connected"** (or temporarily turn off cellular data). |
| **ESP-01 constantly restarts / reboots** | Insufficient 3.3V power during Wi-Fi transmit bursts | Do not power ESP-01 from Arduino 3.3V pin. Use external 3.3V regulator/buck converter with common ground. |
| **Garbage printed on boot / immediate page eject** | 74880-baud ESP bootloader noise leaking into UART | Fixed in v1.2: ESP UART logging is completely muted; Arduino startup delay increased to 1500ms with stray control code (`0x0C`) filtering. Ensure firmware is updated. |
| **`avrdude` fails with sync error when flashing Arduino** | ESP-01 is actively driving Arduino Pin 0 (`RX`) | Tap **Flash Mode (Release UART)** in the Web Portal at `http://192.168.4.1` (or curl `/flash_mode`), then flash Arduino within 90 seconds without unplugging any wires. |
| **Garbled characters printed** | Baud rate mismatch or floating ground | Verify baud rate is set to `115200` on both sketches. Ensure DB25 ground pins (18–25) and Arduino GND are firmly connected. |
| **Text prints in steps / stairs** | Newlines lack carriage returns (`\n` without `\r`) | `AUTO_CR_ON_LF` is enabled by default in `config.h`. Ensure DIP switch `DS1-4` is OFF. |
| **Accented letters (é, á, ñ, ü) garbled** | UTF-8 multi-byte characters not converted | The included `cp437_transcode` engine converts UTF-8 to IBM CP437 automatically. Ensure DIP switches `DS1-7` and `DS1-8` are ON (IBM Set #2). |
| **Printer status shows "PAPER OUT"** | PE line disconnected or paper sensor triggered | Ensure paper is correctly loaded into tractor/friction platen. Check wiring of DB25 Pin 12 to Arduino Pin A2. |
| **Arduino stops receiving data mid-page** | Serial buffer overrun during slow printing | The firmware uses XON/XOFF flow control. Ensure Arduino TX (Pin 1) is wired to ESP-01 RX (Pin 7) through the voltage divider. The driver automatically sends periodic `XON` when idle to recover from transients. |

---

## 7. Firmware Flashing Guide (Using Arduino Uno as Programmer)

You can program both the **Arduino Uno** and the **ESP-01 module** using only the Arduino Uno USB cable (`/dev/ttyACM0`).

### Part A: Flashing the Arduino Uno (`arduino_printer_driver`)

When uploading firmware to the Arduino ATmega328P chip, the ESP8266 must not be driving the serial line:

1. **Temporarily unplug the wire** connected to Arduino **Pin 0 (`RX`)**.
2. Run the upload command:
   ```bash
   arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno arduino_printer_driver
   ```
3. **Plug the wire back into Arduino Pin 0 (`RX`)**.

---

### Part B: Flashing the ESP-01 via Arduino Uno Pass-Through

If you ever need to update the ESP8266 firmware without a standalone USB-UART dongle, the Arduino Uno can act as a transparent 3.3V bridge:

1. **Put the Arduino in Reset:** Connect a jumper wire between Arduino **`RESET`** and Arduino **`GND`** (this keeps the ATmega chip disabled).
2. **Wire the ESP-01 for Programming:**
   - ESP **`VCC`** $\to$ External 3.3V (with 200 µF cap across VCC/GND)
   - ESP **`GND`** $\to$ Arduino **GND**
   - ESP **`CH_PD`** $\to$ **3.3V**
   - ESP **`GPIO0`** $\to$ **GND** *(Puts ESP into Bootloader Flash Mode)*
   - ESP **`TX`** $\to$ Arduino **Pin 1 (`TX`)** *(Connects ESP TX to PC USB RX)*
   - ESP **`RX`** $\to$ Arduino **Pin 0 (`RX`)** *(Connects ESP RX to PC USB TX)*
3. **Power-cycle the ESP-01** (unplug and replug the 3.3V power wire for 1 second).
4. Run the upload command:
   ```bash
   arduino-cli compile --fqbn esp8266:esp8266:generic:FlashMode=dout,eesz=1M64 esp8266_cups_bridge
   python3 ~/.arduino15/packages/esp8266/hardware/esp8266/3.1.2/tools/esptool/esptool.py \
     -p /dev/ttyACM0 -b 115200 --before no_reset write_flash 0x0 \
     ~/.cache/arduino/sketches/*/esp8266_cups_bridge.ino.bin
   ```
5. **Return to Normal Run Mode:**
   - Disconnect ESP **`GPIO0`** from GND and connect it to **3.3V**.
   - Disconnect Arduino **`RESET`** from **`GND`**.
   - Rewire for normal operation: **ESP TX $\to$ Arduino Pin 0 (RX)**, **Arduino Pin 1 (TX) $\to$ ESP RX**.
   - Power-cycle the system.
