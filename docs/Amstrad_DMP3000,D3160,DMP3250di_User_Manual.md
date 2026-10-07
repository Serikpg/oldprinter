# AMSTRAD DMP3000 / DMP3160 / DMP3250di Dot Matrix Printer
## User Manual

> **Applies to Models:** DMP3000, DMP3160, and DMP3250di  
> **Publication:** Part No. 802D0201-3251 | First Edition 1986, Second Edition 1987  
> **Author:** Written by Ivor Spital | Typeset and published by AMSTRAD Plc.  
> **Copyright:** © Copyright 1986, 1987 AMSTRAD Plc. All rights reserved.

---

## Addendum Sheet for Users with 464/6128 BASIC

### AMSTRAD BASIC Aids to Printer Operation

#### Print Formatting
BASIC print format commands such as `PRINT USING`, `PRINT TAB`, and `PRINT SPC` can be directed to the printer simply by adding the `#8` stream director instead of using the command `LPRINT`. Equally, the use of the semicolon and comma in `PRINT #8` statements will enable successive expressions to be printed adjacent to one another, or in adjacent print zones. The `ZONE` command applies to both the screen and the printer.

**Example commands:**
```basic
10 ZONE 26
20 PRINT #8,"text";"semicolon","comma"
30 PRINT #8,TAB(30)"column 30"
40 PRINT #8,"Leave";SPC(10)"ten spaces"
50 PRINT #8,USING"**$##.## to the pound";1.3975
```

#### The WIDTH Command
You may use the `WIDTH` command to specify the number of characters per line (in the range 1 to 254) to be printed (the computer defaults to 132).

**Example command:**
```basic
10 WIDTH 50
20 PRINT #8,STRING$(200,42)
```

The command `WIDTH 255` selects 'unlimited' line wrapping, and you should use this setting when performing graphics printing (explained later in this manual).

> **Note:** The `MODE` in which the computer is operating (i.e. 20, 40 or 80 column) bears no relationship to the size or number of characters per line on the printer.

#### The POS Function
The form `POS(#8)` may be used to determine the next print-position on the paper. Note that this does not necessarily correspond to the physical position of the print head.

**Example command:**
```basic
10 CLS
20 PRINT #8,"123456789";
30 PRINT POS(#8): REM display the print-position on the screen
40 PRINT #8: REM flush buffer
```

---

## Dust Cover Notice

**English:**  
Your new Amstrad DMP printer has been supplied to you with a new improved dust cover.
- **Fitting & Removal:** To prevent damage to the hinges hold the cover vertically when removing or fitting the cover.
- **Cutting Edge:** This allows a sheet of continuous paper to be torn off cleanly along the perforated line.  
*(© 1990 Amstrad plc SJM 257-90 | 6162001-41116)*

**Français:**  
Votre nouvelle imprimante Amstrad DMP vous est fournie avec un nouveau couvercle anti-poussière amélioré.
- **Installation et Désinstallation:** Pour éviter d'endommager les charnières, maintenez le couvercle verticalement lorsque vous l'enlevez ou l'installez.
- **Coupe Papier:** Cela permet de déchirer proprement une feuille de papier continu le long de la ligne perforée.

**Español:**  
Su impresora Amstrad DMP se suministra ahora con una cubierta de nuevo diseño.
- **Cómo montar y desmontar la cubierta:** Para evitar que se dañen los goznes, realice estas operaciones manteniendo la cubierta en vertical.
- **Borde cortador:** El borde posterior de la cubierta permite cortar cómodamente el papel continuo.

---

## Introduction

### AMSTRAD DMP3000/3160 PC Compatible Dot Matrix Printer

The DMP3000/3160 is yet another milestone in the AMSTRAD range of low-cost high-performance computer products.

It combines the versatility of an industry standard software instruction set with AMSTRAD's expertise in quality engineering.

Single cut sheet or continuous paper may be used, and the ingenious 'flatbed' design allows the easy insertion and alignment of both tractor and friction feed paper. Printing speeds of up to 160 characters per second will make rapid work of even the most lengthy drafts.

The extremely wide choice of sizes and typefaces coupled with a complete ASCII, international and graphics character set should provide a solution to any printing problem. In addition, the implementation of dot addressable graphics and standard Epson compatible command codes will allow the DMP3000/3160 to operate directly with most software, including word processors, spreadsheets and graphics programs.

The DMP3000/3160 will operate with the AMSTRAD PC or any other IBM PC-compatible which incorporates a standard parallel printer interface.

The DMP3000/3160 will also operate with any other personal or home computer (for example the AMSTRAD CPC series or the Acorn range of BBC microcomputers) which provide standard parallel printer output. In addition, the printer may be used (via a suitable interface) with computers which provide serial printer output (for example the Commodore or Sinclair ZX Spectrum range of computers).

> **NOTE:** Throughout this manual, all references to model **DMP3000** are equally applicable to model **DMP3160** and **DMP3250di** (except where otherwise stated).

### Maintenance, Servicing & Notices
All maintenance and service on the product must be carried out by AMSTRAD authorised dealers. AMSTRAD cannot accept any liability whatsoever for any loss or damage caused by service or maintenance by unauthorised personnel. This guide is intended only to assist the reader in the use of the product, and therefore, AMSTRAD shall not be liable for any loss or damage whatsoever arising from the use of any information or particulars in, or any error or omission in, this guide or any incorrect use of the product.

We ask that all users take care to submit their user registration/guarantee cards.

**Correspondence:**  
AMSTRAD INFORMATION CENTRE  
1 St. James's Road  
BRENTWOOD  
Essex CM14 4LF  
Telephone: 0277 230222 | Fax: 0277 222117

**Trademarks:**  
- IBM, IBM PC, IBM BASIC, and DOS are trademarks of International Business Machines Inc.  
- MS-DOS and Microsoft BASIC are trademarks of Microsoft Corporation.  
- DOS Plus, GEM, and CP/M are trademarks of Digital Research Inc.  
- Locomotive BASIC 2 is the trademark of Locomotive Software Ltd.  
- Acknowledgements to Acorn, BBC, CBM, Centronics and Epson.

---

## IMPORTANT: You Must Read This...

1. Always connect the mains lead of the printer to a 3-pin plug following the instructions in Chapter 1.
2. Do not attempt to connect the printer to any mains supply other than **220-240V AC 50Hz**.
3. There are no user serviceable parts inside the printer — **DO NOT ATTEMPT TO GAIN ACCESS INSIDE THE CASING**. Refer all servicing to qualified service personnel.
4. Do not operate the printer with its ribbon removed.
5. Do not operate the printer with no paper loaded.
6. Do not switch on or operate the printer with the cardboard print head stabilisers in position.
7. Do not bring drinks or any other liquids near the printer. If you do accidentally spill liquid on the printer, immediately remove the mains plug from the supply socket and consult your dealer.
8. Do not block or cover the ventilation slots in the cabinet.
9. Do not use or store the printer in excessively hot, cold, damp, or dusty areas.

---

## Table of Contents

- [Chapter 1: Open the box...](#chapter-1-open-the-box)
  - [How to wire up the mains plug](#how-to-wire-up-the-mains-plug)
  - [Preparing the printer](#preparing-the-printer)
  - [Fitting the ink ribbon](#fitting-the-ink-ribbon)
  - [Connecting the printer to your computer](#connecting-the-printer-to-your-computer)
  - [Loading the paper](#loading-the-paper)
  - [How the controls work](#how-the-controls-work)
  - [First steps in printing](#first-steps-in-printing)
  - [How to load tractor feed paper](#how-to-load-tractor-feed-paper)
- [Chapter 2: Simple printing exercises...](#chapter-2-simple-printing-exercises)
  - [Printing and listing in BASIC](#printing-and-listing-in-basic)
  - [Notation used in this manual](#notation-used-in-this-manual)
  - [Printing DOS files](#printing-dos-files)
  - [Wildcards](#wildcards)
  - [Listing the disk directory to the printer](#listing-the-disk-directory-to-the-printer)
  - [Echoing screen output to the printer](#echoing-screen-output-to-the-printer)
  - [Printing a screen dump](#printing-a-screen-dump)
  - [Printing GEM files](#printing-gem-files)
  - [Printing DOS Plus and CP/M files](#printing-dos-plus-and-cpm-files)
  - [The print buffer](#the-print-buffer)
  - [Default character set](#default-character-set)
  - [The DIP switches](#the-dip-switches)
  - [How to print international characters](#how-to-print-international-characters)
  - [How to change to an alternative typeface](#how-to-change-to-an-alternative-typeface)
  - [Control codes](#control-codes)
- [Chapter 3: Selecting print styles...](#chapter-3-selecting-print-styles)
  - [Choice of styles](#choice-of-styles)
  - [Selecting one of the main typefaces](#selecting-one-of-the-main-typefaces)
  - [Selecting additional functions](#selecting-additional-functions)
  - [Selecting underline or double-width printing](#selecting-underline-or-double-width-printing)
  - [Combining styles](#combining-styles)
  - [Subscripts and superscripts](#subscripts-and-superscripts)
  - [Illegal combinations — What you can and can't do](#illegal-combinations--what-you-can-and-cant-do)
  - [Table of Combined Print Styles](#table-of-combined-print-styles)
- [Chapter 4: Print formatting control...](#chapter-4-print-formatting-control)
  - [Print head movement](#print-head-movement)
  - [Form feed](#form-feed)
  - [Margins](#margins)
  - [Page length setting](#page-length-setting)
  - [Skip perforation setting](#skip-perforation-setting)
  - [Tabulation](#tabulation)
  - [Paper feed rates](#paper-feed-rates)
- [Chapter 5: Graphics printing...](#chapter-5-graphics-printing)
  - [What is graphics printing?](#what-is-graphics-printing)
  - [Calculating parameters <n1> and <n2>](#calculating-parameters-n1-and-n2)
  - [Graphics modes (8-pin)](#graphics-modes-8-pin)
  - [Table of 8-Pin Graphics Modes](#table-of-8-pin-graphics-modes)
  - [9-pin bit image mode](#9-pin-bit-image-mode)
  - [Bit image mode selection/change](#bit-image-mode-selectionchange)
  - [Graphics character alignment](#graphics-character-alignment)
- [Chapter 6: Extra functions...](#chapter-6-extra-functions)
  - [Incremental print](#incremental-print)
  - [Printable code area expansion](#printable-code-area-expansion)
  - [Eighth bit setting](#eighth-bit-setting)
  - [Control code printing](#control-code-printing)
  - [Reset printer, paper out sensor, bell, delete](#reset-printer-paper-out-sensor-bell-delete)
  - [Print head control](#print-head-control)
  - [Character table selection](#character-table-selection)
  - [International character set selection](#international-character-set-selection)
  - [Print mode selection](#print-mode-selection)
  - [User defined characters (Download characters)](#user-defined-characters-download-characters)
  - [Hexadecimal dump mode](#hexadecimal-dump-mode)
- [Chapter 7: For your reference...](#chapter-7-for-your-reference)
  - [Technical Specification](#technical-specification)
  - [Printer socket](#printer-socket)
  - [Interface Pin Assignment (Centronics 36-Pin)](#interface-pin-assignment-centronics-36-pin)
  - [DIP Switch Functions](#dip-switch-functions)
  - [Signal Timing](#signal-timing)
- [Appendix 1: Table of Control Codes](#appendix-1-table-of-control-codes)
- [Appendix 2: Character Tables](#appendix-2-character-tables)
  - [Table 1: Epson FX - Standard](#table-1-epson-fx---standard)
  - [Table 2: Epson FX - NLQ](#table-2-epson-fx---nlq)
  - [Table 3.1: IBM Character Set #1 - Standard](#table-31-ibm-character-set-1---standard)
  - [Table 3.2: IBM Character Set #2 - Standard](#table-32-ibm-character-set-2---standard)
  - [Table 4.1: IBM Character Set #1 - NLQ](#table-41-ibm-character-set-1---nlq)
  - [Table 4.2: IBM Character Set #2 - NLQ](#table-42-ibm-character-set-2---nlq)
  - [International Character Sets Table](#international-character-sets-table)
- [Appendix 3: Index](#appendix-3-index)

---


# Chapter 1: Open the box...

### Subjects covered in this chapter:
- How to wire up the mains plug
- Preparing the printer
- Fitting the ink ribbon
- Connecting the printer to your computer
- Loading the paper
- How the controls work
- First steps in printing
- How to load tractor feed paper

---

### How to wire up the mains plug

As the colours of the wires in the mains lead of this apparatus may not correspond with the coloured markings identifying the terminals in your plug, proceed as follows:

- The wire which is coloured **GREEN-AND-YELLOW** must be connected to the terminal in the plug which is marked by the letter **E** or by the safety earth symbol $\frac{1}{=}$ or coloured green or green-and-yellow.
- The wire which is coloured **BLUE** must be connected to the terminal which is marked with the letter **N** or coloured black.
- The wire which is coloured **BROWN** must be connected to the terminal which is marked with the letter **L** or coloured red.

> **WARNING — THIS APPARATUS MUST BE EARTHED.**  
> If in doubt, consult a competent electrician.  
> **FUSE:** If a 13 Amp (BS 1363) plug is used, a **3 Amp** fuse must be fitted. If any other type of plug is used, protect with a 5 Amp fuse either in the plug or adaptor or at the distribution board.

---

### Preparing the printer

1. Carefully unpack the printer from its carton. Retain the packing material in case the printer needs to be transported in the future.
2. Remove the plastic wrapping from the printer.
3. Open the smoked transparent plastic printer cover.
4. **Remove the cardboard print head stabilisers / transit clips:**  
   Before using the printer, make sure to remove the cardboard packing piece(s) holding the print head carriage securely during transit.  
   > **CAUTION:** Do not switch on or operate the printer with the transit stabilisers in place, as this may damage the printer mechanism.
5. **Printer Legs:** Two extension legs are supplied. These may be fitted into the slots in the underside of the printer cabinet if you wish to stand the printer over continuous stationery.
6. **Paper Guide Bar:** A black wire paper guide bar is supplied, which can be fitted between the front feet of the printer to help route continuous stationery.

---

### Fitting the ink ribbon

Carefully fit the ribbon cassette as described below:

1. Firstly, turn the printer away from you so that you are looking at the front of the printer with the cover open.
2. Move the print head carriage gently to the middle of the printer mechanism.
3. Take the ribbon cassette out of its packaging. Notice the ribbon tension knob on the top of the cassette. Turn this knob clockwise in the direction of the arrow to take up any slack in the ribbon.
4. Hold the ribbon cassette with the tension knob uppermost and the exposed ribbon facing toward the rear (toward the print head).
5. Start by fitting the plastic end which is in your right hand: place the bottom of the plastic end into the square hole provided on the printer frame, then press down the left-hand end until it clicks securely into position.
6. The ribbon which runs between the plastic ends must now be correctly positioned between the nose of the print head and the ribbon mask / metal platen. Guide the ribbon carefully into the narrow slot in front of the print head pins.
7. Turn the ribbon tension knob clockwise again to ensure the ribbon feeds smoothly and is taut without any folds or twists.

---

### Connecting the printer to your computer

1. Ensure that **both your computer and the DMP3000 are switched OFF** before making any cable connections.
2. Plug the 36-pin Centronics-type parallel connector of your printer cable into the printer interface socket at the rear of the printer. Snap the two wire retaining clips inward to secure the connector.
3. Connect the other end of the cable to the parallel printer port of your computer:
   - For an **IBM PC or compatible (e.g. Amstrad PC1512 / PC1640):** Connect the 25-pin D-type connector (AMSTRAD Lead **PL-1**) to the computer's standard LPT1 parallel port.
   - For an **AMSTRAD CPC464 / CPC664 / CPC6128:** Connect the ribbon cable edge-connector / Centronics connector (AMSTRAD Lead **PL-2**) to the printer port socket at the rear of the CPC.
   - For other computers (such as BBC Micro, Commodore, Sinclair Spectrum with parallel interface): Connect via their standard parallel printer cable.

---

### Loading the paper

The DMP3000 features a versatile flatbed mechanism supporting both **friction feed** (cut sheets and single pages) and **tractor feed** (fan-fold continuous stationery).

#### Paper Controls
- **FRICTION/TRACTOR Switch:** Located on the top-right of the chassis. Set to **FRICTION** for single cut sheets, or **TRACTOR** for continuous fan-fold paper.
- **Manual Paper Feed Knob:** Located on the right-hand side of the printer casing. Rotate clockwise to advance the paper manually.
- **Paper Thickness Adjustment Lever:** Located inside near the print head. Move upwards for thicker paper (or multi-part copies), downwards for normal/thinner paper.
  - Moving the knob downward positions the print head closer to the paper, producing darker printing.
  - Moving it upward moves the print head further away, accommodating multi-sheet stationery and preventing ribbon smudging.

#### How to load plain paper (Friction Feed)
1. Set the **FRICTION/TRACTOR switch to TRACTOR**.
2. Take a sheet of plain paper (A4 or similar) and slide it down into the lower paper guide slots behind the platen.
3. You will see the paper emerging from underneath the platen roller up toward the print head.
4. Straighten the paper by hand so it is aligned squarely.
5. Set the **FRICTION/TRACTOR switch to FRICTION**. The platen rollers will now grip the paper firmly.
6. Advance the paper to the desired top-of-form margin using the manual feed knob or the Line Feed button.

---

### How the controls work

The printer's main controls and indicators are situated on the top and right side of the unit:

| Control / Indicator | Type | Function |
| :--- | :--- | :--- |
| **Mains ON/OFF Switch** | Rocker Switch (Right side) | Turns mains power to the printer ON or OFF. |
| **POWER Lamp** | Indicator LED (Green/Red) | Illuminates when the printer is connected to the mains and switched on. |
| **READY Lamp** | Indicator LED | Indicates the printer is powered, initialised, and ready to accept data. |
| **PAPER OUT Lamp** | Indicator LED | Lights up and sounds an alarm bleeper when the paper runs out. |
| **ON LINE Lamp** | Indicator LED | Illuminates when the printer is in the ON LINE condition. |
| **ON LINE Button** | Push-button | Toggles the printer between **ON LINE** (ready to accept data and print) and **OFF LINE** (pauses printing; enables paper feed buttons). |
| **LF (Line Feed) Button** | Push-button | Advances the paper by one line when pressed once; advances continuously when held down. *(Only functions when printer is OFF LINE).* |
| **FF (Form Feed) Button** | Push-button | Advances the paper to the top of the next page (one whole page length). *(Only functions when printer is OFF LINE).* |

> **Operating Rule:**
> - **ON LINE:** Ready to print data from the computer. Paper feed buttons (LF, FF) are disabled.
> - **OFF LINE:** Printing paused. LF and FF buttons are active to allow manual paper movement.

---

### First steps in printing

#### Self Test Printing
The DMP3000 has a built-in self-test function that exercises the internal microprocessor, print head pins, and character generator by printing out the full ASCII character set continuously:

1. Ensure paper and ribbon are loaded.
2. Switch the mains **ON/OFF switch to OFF**.
3. Hold down the **LF button** and switch the mains switch to **ON**.
4. Release the LF button. The printer will immediately begin printing self-test lines of characters.
5. Check that all character dots are sharp, clear, and evenly inked across the line.
6. Switch the printer **OFF** to terminate the self test.

#### Switching On Normally
When switched on with paper loaded:
1. The print head carriage will initialise and move to its home position.
2. The **POWER** and **ON LINE** lamps will illuminate.
3. The printer is immediately ready to receive data from your PC.

#### Printing Your First Word in BASIC
Start up BASIC on your computer (IBM BASIC, GW-BASIC, Locomotive BASIC 2, etc.) and type:
```basic
LPRINT "hello"
```
Press the `[Return]` or `[Enter]` key. The word **hello** will be printed immediately by the DMP3000.

---

### How to load tractor feed paper

Continuous fan-fold stationery is ideal for long program listings, spreadsheets, and batch reports:

1. Open or remove the printer cover.
2. Hinge back the tractor flaps (tractor covers) on the two outer sliding plastic tractor units to expose the paper-locating sprockets/cogs.
3. Release the tractor lock levers and slide the left and right tractors sideways so that their spacing matches the width of your continuous paper.
4. Place the perforated holes on the left and right edges of the paper over the tractor cogs.
5. Close both tractor covers securely over the paper pins.
6. Set the **FRICTION/TRACTOR switch to TRACTOR**.
7. Adjust the paper tension so the paper lies flat without bowing or tearing at the perforations, then lock the tractor levers.
8. Use the **LF button** or the manual feed knob to advance the top edge of the paper past the print head.
9. Close the printer cover.

#### Using Extension Legs and Paper Guide Bar
- Snap the two extension legs into the bottom of the printer casing to elevate the chassis.
- Place the box of continuous paper directly underneath the printer.
- Fit the metal wire paper guide bar between the front feet to separate incoming feed paper from the printed output paper stacking behind the printer.

# Chapter 2: Simple printing exercises...

### Subjects covered in this chapter:
- Printing and listing in BASIC
- Notation used in this manual
- Printing DOS files
- Wildcards
- Listing the disk directory to the printer
- Echoing screen output to the printer
- Printing a screen dump
- Printing GEM files
- Printing DOS Plus and CP/M files
- The print buffer
- Default character set
- The DIP switches
- How to print international characters
- How to change to an alternative typeface
- Control codes

---

### Printing and listing in BASIC

In BASIC, text and data are directed to the printer using the `LPRINT` and `LLIST` commands.

```basic
10 LPRINT "This text will be printed on the DMP3000"
20 LPRINT 12345
```

#### Suppressing Carriage Return and Line Feed
Normally, each `LPRINT` statement sends a Carriage Return (CR) and Line Feed (LF) at the end of the line. You can suppress the automatic CR/LF by placing a semicolon (`;`) or comma (`,`) at the end of the statement:

```basic
10 LPRINT 123;
20 LPRINT 456,
30 LPRINT 789
```
**Output:**
```
123 456   789
```
- A **semicolon (`;`)** prints the next item immediately adjacent to the preceding item.
- A **comma (`,`)** advances the print head to the start of the next preset print zone.
- An unsuppressed `LPRINT` statement (line 30) outputs the entire line and flushes the print buffer.

#### Listing Programs
To list the entire program currently stored in computer memory directly to the printer, type:
```basic
LLIST
```

---

### Notation used in this manual

To make control codes and escape sequences unambiguous, standard ASCII names and delimiters are used throughout this manual:

- **`<n>` or `<parameter>`:** Represents a numeric variable parameter or ASCII character code (usually specified as `CHR$(n)` in BASIC).
- **`NUL`:** ASCII code 0 (`CHR$(0)`).
- **`SOH`:** ASCII code 1 (`CHR$(1)`).
- **`ESC`:** ASCII Escape code 27 (`CHR$(27)`).
- **`BEL`:** ASCII Bell code 7 (`CHR$(7)`).
- **`BS`:** ASCII Backspace code 8 (`CHR$(8)`).
- **`HT`:** ASCII Horizontal Tab code 9 (`CHR$(9)`).
- **`LF`:** ASCII Line Feed code 10 (`CHR$(10)`).
- **`VT`:** ASCII Vertical Tab code 11 (`CHR$(11)`).
- **`FF`:** ASCII Form Feed code 12 (`CHR$(12)`).
- **`CR`:** ASCII Carriage Return code 13 (`CHR$(13)`).
- **`SO`:** ASCII Shift Out code 14 (`CHR$(14)`).
- **`SI`:** ASCII Shift In code 15 (`CHR$(15)`).
- **`DC1` to `DC4`:** Device Control codes 17 to 20 (`CHR$(17)` to `CHR$(20)`).
- **`CAN`:** ASCII Cancel code 24 (`CHR$(24)`).
- **`DEL`:** ASCII Delete code 127 (`CHR$(127)`).

---

### Printing DOS files

Under MS-DOS / PC-DOS, files can be sent to the printer using standard operating system commands:

1. **Using COPY:**
   ```cmd
   COPY filename.txt PRN
   ```
   *(or `COPY filename.txt LPT1:`)*
2. **Using TYPE with redirection:**
   ```cmd
   TYPE filename.txt > PRN
   ```
3. **Using the PRINT command:**
   ```cmd
   PRINT filename.txt
   ```
   *(This initiates background print spooling).*

---

### Wildcards

DOS wildcard characters `*` (matching any group of characters) and `?` (matching any single character) can be used when copying or printing multiple files:
```cmd
COPY *.TXT PRN
COPY REPORT?.DOC PRN
```

---

### Listing the disk directory to the printer

To obtain a hard copy printout of the files on your disk drive, redirect the `DIR` command to the printer device `PRN`:
```cmd
DIR > PRN
DIR *.BAS > PRN
```

---

### Echoing screen output to the printer

You can instruct DOS to echo all subsequent text displayed on the screen directly to the printer:

- Press **`[Ctrl]` + `P`** (or **`[Ctrl]` + `[PrtSc]`**).
- From this moment on, everything typed or displayed on the monitor will also be printed out.
- To cancel printer echoing, press **`[Ctrl]` + `P`** again.

---

### Printing a screen dump

To print a snapshot copy of the current text screen:
- Press **`[Shift]` + `[PrtSc]`**.
- To print graphic screens from DOS, first load the DOS graphics screen dump utility by typing:
  ```cmd
  GRAPHICS
  ```
  Once loaded, pressing `[Shift]` + `[PrtSc]` will dump graphic screens to the printer in Epson-compatible dot graphics mode.

---

### Printing GEM files

When using the GEM (Graphics Environment Manager) desktop on the AMSTRAD PC:
1. Select the document or graphic file icon using the mouse pointer.
2. Drag the icon over to the **PRINTER** desktop icon, or select **Print** from the File menu.
3. GEM will convert fonts and graphics into DMP3000 raster commands automatically via its installed printer driver.

---

### Printing DOS Plus and CP/M files

Under Digital Research DOS Plus or CP/M-86 / CP/M-2.2:
- To copy a file to the list device:
  ```cmd
  PIP LST:=filename.ext
  ```
- To toggle printer echo:
  Press **`[Ctrl]` + `P`**.

---

### The print buffer

The DMP3000 contains an internal RAM buffer that stores incoming data before printing. This allows the computer to transfer data quickly and proceed with other tasks.

The print buffer is automatically flushed (emptied onto paper) under the following conditions:
1. **When the buffer becomes full.**
2. **When the printer is switched OFF LINE** (by pressing the ON LINE button).
3. **When a Line Feed (LF), Form Feed (FF), or Carriage Return (CR)** is received.

---

### Default character set

By default from the factory, the DMP3000 is configured to reproduce **IBM Character Set #2** (see Appendix 2, Table 3.2).

You can print out a test of the character set using the following BASIC routine:
```basic
10 FOR n=32 TO 126
20 LPRINT CHR$(n);
30 NEXT
40 :
50 FOR n=160 TO 254
60 LPRINT CHR$(n);
70 NEXT
80 LPRINT
```

---

### The DIP switches

The default character set, interface options, international characters, and line parameters are configured by miniature switches (DIP switches) located at the rear of the printer.

> **IMPORTANT:** Always switch the printer **OFF** before adjusting any DIP switches!

There are two banks of DIP switches:
- **Bank DS1:** Contains **8 switches** (DS1-1 to DS1-8).
- **Bank DS2:** Contains **10 switches** (DS2-1 to DS2-10).

#### Table 2.1: Default Character Set DIP Switch Settings (DS1-7 & DS1-8)

| Character Set | DS1-7 | DS1-8 | Description |
| :--- | :--- | :--- | :--- |
| **Epson FX - standard** | **OFF** | **OFF** | Standard Epson draft character set (Table 1) |
| **Epson FX - NLQ** | **ON** | **OFF** | Near Letter Quality typeface default (Table 2) |
| **IBM Character Set #1** | **OFF** | **ON** | IBM PC character set #1 (Table 3.1) |
| **IBM Character Set #2** | **ON** | **ON** | IBM PC character set #2 with extended symbols (Table 3.2 — *Factory Default*) |

---

### How to print international characters

When the printer is set to standard USA ASCII, typing `LPRINT "£5"` produces `#5` because character code 35 (`#`) is used for the hash sign in the USA set.

To print the British pound sign (`£`) or accented characters for European languages, set DIP switches **DS1-1, DS1-2, and DS1-3**.

> **IMPORTANT:** To print international characters, IBM character set #1 or #2 must **NOT** be selected — i.e. DIP switch **DS1-8 must be OFF**, or `ESC m NUL` must be selected via software.

#### Table 2.2: International Characters DIP Switch Selection

| Country | DS1-1 | DS1-2 | DS1-3 |
| :--- | :--- | :--- | :--- |
| **USA** | **ON** | **ON** | **ON** |
| **France** | **OFF** | **ON** | **ON** |
| **Germany** | **ON** | **OFF** | **ON** |
| **UK** | **OFF** | **OFF** | **ON** |
| **Denmark I** | **ON** | **ON** | **OFF** |
| **Sweden** | **OFF** | **ON** | **OFF** |
| **Italy** | **ON** | **OFF** | **OFF** |
| **Spain I** | **OFF** | **OFF** | **OFF** |

#### Table 2.3: International Character Substitution Matrix

The table below shows the exact character printed for each international country at the 12 substitution character code positions:

| Country | Dec: 35<br>Hex: &23 | Dec: 36<br>Hex: &24 | Dec: 64<br>Hex: &40 | Dec: 91<br>Hex: &5B | Dec: 92<br>Hex: &5C | Dec: 93<br>Hex: &5D | Dec: 94<br>Hex: &5E | Dec: 96<br>Hex: &60 | Dec: 123<br>Hex: &7B | Dec: 124<br>Hex: &7C | Dec: 125<br>Hex: &7D | Dec: 126<br>Hex: &7E |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **USA** | `#` | `$` | `@` | `[` | `\` | `]` | `^` | `` ` `` | `{` | `\|` | `}` | `~` |
| **France** | `#` | `$` | `à` | `°` | `ç` | `§` | `^` | `` ` `` | `é` | `ù` | `è` | `¨` |
| **Germany** | `#` | `$` | `§` | `Ä` | `Ö` | `Ü` | `^` | `` ` `` | `ä` | `ö` | `ü` | `ß` |
| **UK** | `£` | `$` | `@` | `[` | `\` | `]` | `^` | `` ` `` | `{` | `\|` | `}` | `~` |
| **Denmark I** | `#` | `$` | `@` | `Æ` | `Ø` | `Å` | `^` | `` ` `` | `æ` | `ø` | `å` | `~` |
| **Sweden** | `#` | `¤` | `É` | `Ä` | `Ö` | `Å` | `Ü` | `é` | `ä` | `ö` | `å` | `ü` |
| **Italy** | `#` | `$` | `@` | `°` | `\` | `é` | `^` | `ù` | `à` | `ò` | `è` | `ì` |
| **Spain I** | `₧` | `$` | `@` | `¡` | `Ñ` | `¿` | `^` | `` ` `` | `¨` | `ñ` | `}` | `~` |
| **Japan** | `#` | `$` | `@` | `[` | `¥` | `]` | `^` | `` ` `` | `{` | `\|` | `}` | `~` |

---

### How to change to an alternative typeface

The DMP3000 can switch dynamically between draft quality and Near Letter Quality (NLQ) printing using software control codes.

#### Selecting NLQ Typeface
```basic
LPRINT CHR$(27) + "x" + CHR$(1)
LPRINT "this is NLQ printing"
```

#### Cancelling NLQ Typeface (Reverting to Standard)
```basic
LPRINT CHR$(27) + "x" + CHR$(0)
LPRINT "this is standard printing"
```

---

### Control codes

A control code is a non-printing character code that instructs the printer to execute an action (such as line feed, bell sound, or switching typeface).

#### Escape Sequences (`ESC`)
The ASCII code 27 (`CHR$(27)`) is the Escape code (`ESC`). An escape sequence consists of `CHR$(27)` followed by a command letter and optional parameters:
- `CHR$(27)`: Alerts the printer that a control command follows.
- `"x"`: The command code letter (e.g. `x` for NLQ mode).
- `CHR$(1)` / `CHR$(0)`: Parameter argument (`1` = ON / Enable, `0` = OFF / Disable).

#### Shorthand Notation
Instead of concatenating `CHR$(1)`, you may send the ASCII character `"1"`:
```basic
LPRINT CHR$(27) + "x1"   : REM Select NLQ
LPRINT CHR$(27) + "x0"   : REM Cancel NLQ
```

# Chapter 3: Selecting print styles...

### Subjects covered in this chapter:
- Choice of styles
- Selecting one of the main typefaces
- Selecting additional functions
- Selecting underline or double-width printing
- Combining styles
- Subscripts and superscripts
- Illegal combinations — What you can and can't do
- Table of Combined Print Styles

---

### Choice of styles

The DMP3000 is capable of over **100 different print style combinations**. There are six main typefaces:

1. **Standard** (Pica — 10 CPI, 80 characters per line)
2. **Mini** (Elite — 12 CPI, 96 characters per line)
3. **Proportional** (Varying character pitch)
4. **Condensed** (17 CPI, 137 characters per line)
5. **NLQ-standard** (Near Letter Quality Pica)
6. **NLQ-proportional** (Near Letter Quality with proportional spacing)

To these main typefaces, you may apply the following additional options:
- **Subscript**
- **Superscript**
- **Double-strike**
- **Italics**
- **Bold** (Emphasized)

Finally, to any valid combination of the above, you may also add:
- **Underline**
- **Double-width**

> **Tip:** You can always return the printer to standard typeface with all options cancelled by switching the printer **OFF**, then **ON** again.

---

### Selecting one of the main typefaces

#### 1. Standard Typeface (Pica)
Automatically selected when the printer is switched on or when other typefaces are cancelled.
- **TO SELECT:** Cancel other typefaces (`ESC P` to cancel mini; `DC2` to cancel condensed; `ESC p NUL` to cancel proportional; `ESC x NUL` to cancel NLQ).

#### 2. Mini Typeface (Elite — 12 CPI)
- **TO SELECT:** `ESC M` (`CHR$(27) + "M"`)
- **TO CANCEL:** `ESC P` (`CHR$(27) + "P"`)
```basic
LPRINT CHR$(27) + "M"
LPRINT "this is mini typeface"
LPRINT CHR$(27) + "P"
LPRINT "this is standard typeface again"
```

#### 3. Proportional Typeface
- **TO SELECT:** `ESC p SOH` (`CHR$(27) + "p" + CHR$(1)`)
- **TO CANCEL:** `ESC p NUL` (`CHR$(27) + "p" + CHR$(0)`)
```basic
LPRINT CHR$(27) + "p" + CHR$(1)
LPRINT "this is proportional typeface"
LPRINT CHR$(27) + "p" + CHR$(0)
LPRINT "this is standard typeface again"
```

#### 4. Condensed Typeface (17 CPI)
- **TO SELECT:** `SI` or `ESC SI` (`CHR$(15)` or `CHR$(27) + CHR$(15)`)
- **TO CANCEL:** `DC2` (`CHR$(18)`)
```basic
LPRINT CHR$(15)
LPRINT "this is condensed typeface"
LPRINT CHR$(18)
LPRINT "this is standard typeface again"
```

#### 5. NLQ-Standard Typeface (Near Letter Quality)
- **TO SELECT:** `ESC x SOH` (`CHR$(27) + "x" + CHR$(1)`)
- **TO CANCEL:** `ESC x NUL` (`CHR$(27) + "x" + CHR$(0)`)
*(Can also be selected at power-on by holding down LF and ON LINE buttons together).*
```basic
LPRINT CHR$(27) + "x" + CHR$(1)
LPRINT "this is NLQ typeface"
LPRINT CHR$(27) + "x" + CHR$(0)
LPRINT "this is standard typeface again"
```

#### 6. NLQ-Proportional Typeface
- **TO SELECT:** `ESC x SOH ESC p SOH` (`CHR$(27) + "x" + CHR$(1) + CHR$(27) + "p" + CHR$(1)`)
- **TO CANCEL:** `ESC x NUL ESC p NUL` (`CHR$(27) + "x" + CHR$(0) + CHR$(27) + "p" + CHR$(0)`)
```basic
LPRINT CHR$(27) + "x" + CHR$(1) + CHR$(27) + "p" + CHR$(1)
LPRINT "this is NLQ-proportional typeface"
LPRINT CHR$(27) + "x" + CHR$(0) + CHR$(27) + "p" + CHR$(0)
LPRINT "this is standard typeface again"
```

---

### Selecting additional functions

#### Subscript Option
- **TO SELECT:** `ESC S SOH` (`CHR$(27) + "S" + CHR$(1)`)
- **TO CANCEL:** `ESC T` (`CHR$(27) + "T"`)

#### Superscript Option
- **TO SELECT:** `ESC S NUL` (`CHR$(27) + "S" + CHR$(0)`)
- **TO CANCEL:** `ESC T` (`CHR$(27) + "T"`)

#### Double Strike Option
- **TO SELECT:** `ESC G` (`CHR$(27) + "G"`)
- **TO CANCEL:** `ESC H` (`CHR$(27) + "H"`)

#### Italics Option
- **TO SELECT:** `ESC 4` (`CHR$(27) + "4"`)
- **TO CANCEL:** `ESC 5` (`CHR$(27) + "5"`)

#### Bold Option (Emphasized)
- **TO SELECT:** `ESC E` (`CHR$(27) + "E"`)
- **TO CANCEL:** `ESC F` (`CHR$(27) + "F"`)

---

### Selecting underline or double-width printing

#### Underline Option
- **TO SELECT:** `ESC - SOH` (`CHR$(27) + "-" + CHR$(1)`)
- **TO CANCEL:** `ESC - NUL` (`CHR$(27) + "-" + CHR$(0)`)

#### Double-Width Option
- **Single-Line Double-Width:**  
  - **TO SELECT:** `SO` or `ESC SO` (`CHR$(14)` or `CHR$(27) + CHR$(14)`)  
  - **TO CANCEL:** `DC4` (`CHR$(20)`) or automatically cancelled by a Line Feed.
- **Continuous Double-Width:**  
  - **TO SELECT:** `ESC W SOH` (`CHR$(27) + "W" + CHR$(1)`)  
  - **TO CANCEL:** `ESC W NUL` (`CHR$(27) + "W" + CHR$(0)`)

---

### Combining styles

To simplify program code when combining multiple styles, define control strings in variables at the beginning of your program:

```basic
10 REM Printer Control Code Definitions
20 e$ = CHR$(27)              : REM Escape
30 s$ = CHR$(1)               : REM On (SOH)
40 n$ = CHR$(0)               : REM Off (NUL)
50 ms$ = e$ + "M"             : REM Mini Select
60 mc$ = e$ + "P"             : REM Mini Cancel
70 is$ = e$ + "4"             : REM Italics Select
80 ic$ = e$ + "5"             : REM Italics Cancel
90 us$ = e$ + "-" + s$        : REM Underline Select
100 uc$ = e$ + "-" + n$       : REM Underline Cancel
110 ws$ = CHR$(14)            : REM Double Width Select
120 wc$ = CHR$(20)            : REM Double Width Cancel
130 cs$ = CHR$(15)            : REM Condensed Select
140 cc$ = CHR$(18)            : REM Condensed Cancel
150 nss$ = e$ + "x" + s$      : REM NLQ Select
160 nsc$ = e$ + "x" + n$      : REM NLQ Cancel
170 sbs$ = e$ + "S" + s$      : REM Subscript Select
180 sps$ = e$ + "S" + n$      : REM Superscript Select
190 ssc$ = e$ + "T"           : REM Subscript & Superscript Cancel
200 ps$ = e$ + "p" + s$       : REM Proportional Select
210 pc$ = e$ + "p" + n$       : REM Proportional Cancel
220 ds$ = e$ + "G"            : REM Double Strike Select
230 dc$ = e$ + "H"            : REM Double Strike Cancel
240 bs$ = e$ + "E"            : REM Bold Select
250 bc$ = e$ + "F"            : REM Bold Cancel
```

---

### Illegal combinations — What you can and can't do

Not all typefaces can be combined with all additional options. For example:
- **Mini** and **Condensed** cannot be combined.
- **Bold** is only permitted with **Standard** typeface.
- **Subscript** and **Superscript** cannot be used with **Proportional** or **NLQ-Proportional**.
- **Double Strike** cannot be combined with **NLQ-Standard** or **NLQ-Proportional**.

---

### Table 3.1: Permitted and Illegal Print Style Combinations Matrix

> **NOTES:**
> 1. A blank cell indicates an **illegal combination**.
> 2. **All** permitted combinations may additionally include **Double-Width** and/or **Underlining**.
> 3. When using **Standard Typeface**, you may select both the **Bold** and **Italics** options together.

| Typeface | Sub-Option | NORMAL (OFF) | DOUBLE STRIKE | SUBSCRIPT | SUPERSCRIPT |
| :--- | :--- | :---: | :---: | :---: | :---: |
| **STANDARD TYPEFACE** | **NORMAL (OFF)** | **OK** | **OK** | **OK** | **OK** |
| | **BOLD** | **OK** | **OK** | **OK** | **OK** |
| | **ITALICS** | **OK** | **OK** | **OK** | **OK** |
| **MINI TYPEFACE** | **NORMAL (OFF)** | **OK** | **OK** | **OK** | **OK** |
| | **BOLD** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |
| | **ITALICS** | **OK** | **OK** | **OK** | **OK** |
| **PROPORTIONAL TYPEFACE** | **NORMAL (OFF)** | **OK** | **OK** | *(illegal)* | *(illegal)* |
| | **BOLD** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |
| | **ITALICS** | **OK** | **OK** | *(illegal)* | *(illegal)* |
| **CONDENSED TYPEFACE** | **NORMAL (OFF)** | **OK** | **OK** | **OK** | **OK** |
| | **BOLD** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |
| | **ITALICS** | **OK** | **OK** | **OK** | **OK** |
| **NLQ-STANDARD TYPEFACE** | **NORMAL (OFF)** | **OK** | *(illegal)* | **OK** | **OK** |
| | **BOLD** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |
| | **ITALICS** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |
| **NLQ-PROPORTIONAL TYPEFACE** | **NORMAL (OFF)** | **OK** | *(illegal)* | *(illegal)* | *(illegal)* |
| | **BOLD** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |
| | **ITALICS** | *(illegal)* | *(illegal)* | *(illegal)* | *(illegal)* |

# Chapter 4: Print formatting control...

### Subjects covered in this chapter:
- Print head movement
- Form feed
- Margins
- Page length setting
- Skip perforation setting
- Tabulation
- Paper feed rates

---

### Print head movement

#### Carriage Return (`CR`)
Sends the print head back to the beginning of the line (left margin).
- **TO SELECT:** `CR` (`CHR$(13)`)

#### Line Feed (`LF`)
Advances the paper by one line and flushes the print buffer.
- **TO SELECT:** `LF` (`CHR$(10)`)

#### Backspace (`BS`)
Moves the print head one character position to the left.
- **TO SELECT:** `BS` (`CHR$(8)`)
> **Note:** Backspace will not operate during proportional printing.

---

### Form feed (`FF`)

Advances the paper to the top-of-form on the next page.
- **TO SELECT:** `FF` (`CHR$(12)`)

---

### Margins

#### Left Margin Setting
Sets the left margin to column `<n>` (range 0 to 255 character columns from the left physical edge).
- **TO SELECT:** `ESC l <n>` (`CHR$(27) + "l" + CHR$(n)`)
```basic
LPRINT CHR$(27) + "l" + CHR$(20)   : REM Left margin at column 20
```

#### Right Margin Setting
Sets the right margin to column `<n>` (range 1 to 255 character columns from the left physical edge).
- **TO SELECT:** `ESC Q <n>` (`CHR$(27) + "Q" + CHR$(n)`)
```basic
LPRINT CHR$(27) + "Q" + CHR$(70)   : REM Right margin at column 70
```
> **Note:** If the right margin is set to a value less than or equal to the left margin, the right margin setting is ignored.

---

### Page length setting

#### Page Length by Lines
Sets the page length to `<n>` lines (range 1 to 127).
- **TO SELECT:** `ESC C <n>` (`CHR$(27) + "C" + CHR$(n)`)
```basic
LPRINT CHR$(27) + "C" + CHR$(66)   : REM 66 lines per page (standard 11")
```

#### Page Length by Inches
Sets the page length to `<n>` inches (range 1 to 22).
- **TO SELECT:** `ESC C NUL <n>` (`CHR$(27) + "C" + CHR$(0) + CHR$(n)`)
```basic
LPRINT CHR$(27) + "C" + CHR$(0) + CHR$(11)  : REM 11 inches page length
LPRINT CHR$(27) + "C" + CHR$(0) + CHR$(12)  : REM 12 inches page length
```

---

### Skip perforation setting

Instructs the printer to leave a blank unprinted margin of `<n>` lines at the bottom and top of each page across the paper perforations.
- **TO SELECT:** `ESC N <n>` (`CHR$(27) + "N" + CHR$(n)`)  
  *(where `<n>` is in the range 1 to 127 lines).*
- **TO CANCEL:** `ESC O` (`CHR$(27) + "O"`) *(capital letter O).*
```basic
LPRINT CHR$(27) + "N" + CHR$(6)    : REM Skip 6 lines at perforations
LPRINT CHR$(27) + "O"              : REM Cancel perforation skip
```

---

### Tabulation

#### Horizontal Tabs
Sets up to 32 horizontal tab stop columns in ascending order, terminated with `NUL`.
- **TO SET:** `ESC D <n1> <n2> ... <n32> NUL`
  ```basic
  LPRINT CHR$(27) + "D" + CHR$(10) + CHR$(20) + CHR$(40) + CHR$(0)
  ```
- **TO JUMP:** `HT` (`CHR$(9)`)  
  *(Defaults to every 8 columns upon power-up).*

#### Vertical Tabs
Sets up to 16 vertical line tab positions in ascending order, terminated with `NUL`.
- **TO SET:** `ESC B <n1> <n2> ... <n16> NUL`
  ```basic
  LPRINT CHR$(27) + "B" + CHR$(10) + CHR$(20) + CHR$(0)
  ```
- **TO JUMP:** `VT` (`CHR$(11)`)

#### Tab Channels
The DMP3000 provides **8 independent vertical tab channels** (channels 0 to 7) to store different page layout forms:
- **TO SET TABS IN CHANNEL:** `ESC b <channel> <n1> <n2> ... NUL`  
  *(where `<channel>` is 0 to 7).*
  ```basic
  LPRINT CHR$(27) + "b" + CHR$(2) + CHR$(10) + CHR$(25) + CHR$(40) + CHR$(0)
  ```
- **TO SELECT CHANNEL:** `ESC / <channel>`
  ```basic
  LPRINT CHR$(27) + "/" + CHR$(2)   : REM Select vertical tab channel 2
  ```
  *(If no channel has been selected, channel 0 is used by default).*

---

### Paper feed rates

| Feed Rate | Command | BASIC String | Description |
| :--- | :--- | :--- | :--- |
| **1/8 inch** | `ESC 0` | `CHR$(27) + "0"` | High-density line spacing (8 lines/inch) |
| **7/72 inch** | `ESC 1` | `CHR$(27) + "1"` | 7/72 inch line spacing |
| **1/6 inch** | `ESC 2` | `CHR$(27) + "2"` | Standard default spacing (6 lines/inch) |
| **Variable `<n>`/216 inch** | `ESC 3 <n>` | `CHR$(27) + "3" + CHR$(n)` | `<n>` in range 0 to 255 |
| **Variable `<n>`/72 inch** | `ESC A <n>` | `CHR$(27) + "A" + CHR$(n)` | `<n>` in range 0 to 85 |
| **One-shot forward `<n>`/216 inch** | `ESC J <n>` | `CHR$(27) + "J" + CHR$(n)` | Once-only advance (`<n>` = 0 to 255) |
| **One-shot reverse `<n>`/216 inch** | `ESC j <n>` | `CHR$(27) + "j" + CHR$(n)` | Once-only reverse feed (`<n>` = 0 to 255) |

> **WARNING:** Do not attempt a reverse paper feed while printing within the top **30 mm** or the bottom **80 mm** of cut-sheet paper, or within **30 mm** of perforations on continuous stationery.

# Chapter 5: Graphics printing...

### Subjects covered in this chapter:
- What is graphics printing?
- Calculating parameters `<n1>` and `<n2>`
- Graphics modes (8-pin)
- Table of 8-Pin Graphics Modes
- 9-pin bit image mode
- Bit image mode selection/change
- Graphics character alignment

---

### What is graphics printing?

In graphics mode, the printer stops interpreting data as ASCII text characters and instead uses each incoming byte to directly fire the tiny pins inside the print head.

For each byte received, the printer prints a single vertical column of dots. Each bit in the byte corresponds to a physical print wire:
- A binary `1` fires the pin (prints a dot).
- A binary `0` leaves the pin un-fired (blank space).

#### Print Wire to Data Bit Mapping (8-Pin Bit Image)

```
        DATA BYTE BITS                PRINT HEAD WIRES
     (MSB)  Bit 7  -----------------  Wire 8 (Top)
            Bit 6  -----------------  Wire 7
            Bit 5  -----------------  Wire 6
            Bit 4  -----------------  Wire 5
            Bit 3  -----------------  Wire 4
            Bit 2  -----------------  Wire 3
            Bit 1  -----------------  Wire 2
     (LSB)  Bit 0  -----------------  Wire 1 (Bottom of 8 pins)
            (Wire 9 reserved for underline and 9-pin graphics)
```

---

### Calculating parameters `<n1>` and `<n2>`

Graphics commands require two length parameters, `<n1>` and `<n2>`, which specify the total number of graphics dot columns to be printed before reverting to text mode:
$$\text{Total Columns} = n_1 + 256 \times n_2$$

Where:
- $n_1 = \text{Total} \pmod{256}$
- $n_2 = \text{INT}(\text{Total} / 256)$
- Both $n_1$ and $n_2$ are in the range 0 to 255.

**Helper routine in BASIC:**
```basic
10 INPUT "number of dots"; d
20 PRINT "<n1> ="; d MOD 256
30 PRINT "<n2> ="; INT(d / 256)
```

**Example:** To print a screen row of 640 dots:
- $640 / 256 = 2$ remainder $128$
- $n_1 = 128$, $n_2 = 2$
- Command: `LPRINT CHR$(27) + "L" + CHR$(128) + CHR$(2);`

---

### Graphics modes (8-pin)

- **Single Density (60 DPI):** `ESC K <n1> <n2>` (480 dots/line)
- **Double Density (120 DPI):** `ESC L <n1> <n2>` (960 dots/line)
- **Double Speed Double Density (120 DPI):** `ESC Y <n1> <n2>` (960 dots/line, non-adjacent dots)
- **Quadruple Density (240 DPI):** `ESC Z <n1> <n2>` (1920 dots/line)
- **Universal Bit Image Command:** `ESC * <mode> <n1> <n2>`

---

### Table 5.1: 8-Pin Bit Image Graphics Modes Specifications

| `<mode>` | Graphics Type | Total Dots / 8" Line | Connecting Dot Density / 8" | Head Speed (inch/sec)<br>DMP3160 | Head Speed (inch/sec)<br>DMP3000 |
| :---: | :--- | :---: | :---: | :---: | :---: |
| **0** | Single density | 480 | 480 | 16 | 10.5 |
| **1** | Double density | 960 | 960 | 8 | 5.25 |
| **2** | Double speed / double density | 960 | 480 | 16 | 10.5 |
| **3** | Quadruple density | 1920 | 960 | 8 | 5.25 |
| **4** | CRT graphic I | 640 | 640 | 8 | 5.25 |
| **5** | Plotter graphic | 576 | 576 | 13 | 8.7 |
| **6** | CRT graphic II | 720 | 720 | 8 | 5.25 |

---

### 9-pin bit image mode

The DMP3000 can utilize all 9 vertical print wires for high-density graphic plotting.

- **TO SELECT:** `ESC ^ <mode> <n1> <n2>`  
  *(where `<mode>` is 0 for single density or 1 for double density).*

#### Table 5.2: 9-Pin Graphics Modes

| `<mode>` | Max Dots / 8" Line | Density |
| :---: | :---: | :--- |
| **0** | 480 | Single density |
| **1** | 960 | Double density |

#### Data Format for 9-Pin Mode
Two successive bytes must be sent for every vertical column of dots:
- **First Byte:** Pins 8 down to 1 (Bit 7 = Pin 8, Bit 6 = Pin 7, ..., Bit 0 = Pin 1).
- **Second Byte:** Pin 9 (Bit 7 = Pin 9; Bits 6 to 0 are unused).

```
   FIRST BYTE:
     Bit 7 = Pin 8 (top)
     Bit 6 = Pin 7
     Bit 5 = Pin 6
     Bit 4 = Pin 5
     Bit 3 = Pin 4
     Bit 2 = Pin 3
     Bit 1 = Pin 2
     Bit 0 = Pin 1
   SECOND BYTE:
     Bit 7 = Pin 9 (bottom)
     Bits 6..0 = Not used
```

---

### Bit image mode selection/change

Re-assigns one of the standard graphics mode escape letters (`K`, `L`, `Y`, or `Z`) to any of the 7 bit image modes:
- **TO SELECT:** `ESC ? <code> <mode>`  
  *(where `<code>` is `"K"`, `"L"`, `"Y"`, or `"Z"`, and `<mode>` is 0 to 6).*

---

### Graphics character alignment

> **NOTE:** When printing vertical lines, box borders, or continuous graphics across lines, select **uni-directional printing** (`ESC U SOH`). This eliminates slight bidirectional mechanical registration backlash and ensures vertical lines align with perfect precision.

# Chapter 6: Extra functions...

### Subjects covered in this chapter:
- Incremental print
- Printable code area expansion
- Eighth bit setting
- Control code printing
- Reset printer, paper out sensor, bell, delete
- Print head control
- Character table selection
- International character set selection
- Print mode selection
- User defined characters (Download characters)
- Hexadecimal dump mode

---

### Incremental print

When writing a character at a time (e.g. keyboard typing directly to printer), incremental print mode forces the printer to print each character immediately as it is entered rather than waiting for a full line or carriage return:
- **TO SELECT:** `ESC i SOH` (`CHR$(27) + "i" + CHR$(1)`)
- **TO CANCEL:** `ESC i NUL` (`CHR$(27) + "i" + CHR$(0)`)

---

### Printable code area expansion

By default in Epson FX mode, ASCII codes 128 to 159 and 255 are reserved as non-printable control characters.
- **TO EXPAND AREA:** `ESC 6` (`CHR$(27) + "6"`) — Enables printable characters in the range 128 to 159 and character 255.
- **TO CANCEL:** `ESC 7` (`CHR$(27) + "7"`) — Restores non-printable status.

---

### Eighth bit setting

Allows software control of the 8th data bit (MSB) regardless of the computer's interface output:
- **Accept 8th Bit:** `ESC #` (`CHR$(27) + "#"`) — Normal operation; accepts 8th bit as sent.
- **Set 8th Bit to 1:** `ESC >` (`CHR$(27) + ">"`) — Forces bit 7 of all incoming bytes to 1 (accesses upper 128–255 character set).
- **Reset 8th Bit to 0:** `ESC =` (`CHR$(27) + "="`) — Forces bit 7 of all incoming bytes to 0 (restricts data to 0–127).

---

### Control code printing

Allows control codes (ASCII 0 to 31) to be printed as graphic glyphs rather than executed as commands:
- **TO SELECT:** `ESC I SOH` (`CHR$(27) + "I" + CHR$(1)`)
- **TO CANCEL:** `ESC I NUL` (`CHR$(27) + "I" + CHR$(0)`)

---

### Reset printer, paper out sensor, bell, delete

#### Reset Printer
Restores the printer to its initial power-on state, resetting all margins, tabs, typeface selections, and flushing the buffer.
- **COMMAND:** `ESC @` (`CHR$(27) + "@"`)

#### Paper Out Sensor Control
- **Disable Sensor:** `ESC 8` (`CHR$(27) + "8"`) — Ignores paper-out switch (useful for printing right to the bottom edge of single sheets).
- **Enable Sensor:** `ESC 9` (`CHR$(27) + "9"`) — Re-enables paper-out sensing.

#### Bell (Bleeper)
Sounds the printer's internal piezo bleeper.
- **COMMAND:** `BEL` (`CHR$(7)`)

#### Delete Character
Deletes the most recent character from the print buffer before it is printed.
- **COMMAND:** `DEL` (`CHR$(127)`)

#### Clear Buffer
Discards all data currently waiting in the print buffer.
- **COMMAND:** `CAN` (`CHR$(24)`)

#### On Line / Off Line Software Control
- **Deselect Printer (Off Line):** `DC3` (`CHR$(19)`)
- **Select Printer (On Line):** `DC1` (`CHR$(17)`)

---

### Print head control

#### Home Head (One-Line Unidirectional)
Causes the next line to be printed unidirectionally from left to right.
- **COMMAND:** `ESC <` (`CHR$(27) + "<"`)

#### Uni-Directional Printing
Forces all subsequent lines to be printed in the left-to-right direction only (ensures flawless alignment of graphic boxes and tables).
- **TO SELECT:** `ESC U SOH` (`CHR$(27) + "U" + CHR$(1)`)
- **TO CANCEL:** `ESC U NUL` (`CHR$(27) + "U" + CHR$(0)`)

#### Half Speed Printing
Reduces print head speed by 50% for quiet operation and higher dot accuracy.
- **TO SELECT:** `ESC s SOH` (`CHR$(27) + "s" + CHR$(1)`)
- **TO CANCEL:** `ESC s NUL` (`CHR$(27) + "s" + CHR$(0)`)

---

### Character table selection

Overrides the hardware settings of DIP switches DS1-7 and DS1-8 via software:
- **COMMAND:** `ESC m <n>` (`CHR$(27) + "m" + CHR$(n)`)

#### Table 6.1: Character Table Selection (`ESC m <n>`)

| `<n>` | Standard Typeface | NLQ Typeface | Reference |
| :---: | :--- | :--- | :--- |
| **0** | Table 1 (Epson FX - standard) | Table 2 (Epson FX - NLQ) | Appendix 2, Tables 1 & 2 |
| **1** | Table 3.1 (IBM #1 - standard) | Table 4.1 (IBM #1 - NLQ) | Appendix 2, Tables 3.1 & 4.1 |
| **2** | Table 3.2 (IBM #2 - standard) | Table 4.2 (IBM #2 - NLQ) | Appendix 2, Tables 3.2 & 4.2 |

---

### International character set selection

Overrides DIP switches DS1-1, DS1-2, and DS1-3 via software:
- **COMMAND:** `ESC R <n>` (`CHR$(27) + "R" + CHR$(n)`)

#### Table 6.2: International Character Set Selection Codes (`ESC R <n>`)

| `<n>` | Country |
| :---: | :--- |
| **0** | USA |
| **1** | France |
| **2** | Germany |
| **3** | UK |
| **4** | Denmark I |
| **5** | Sweden |
| **6** | Italy |
| **7** | Spain I |
| **8** | Japan |

---

### Print mode selection

A single master escape command that selects multiple common font attributes using a bit-significant parameter byte `<n>`:
- **TO SELECT:** `ESC ! <n>` (`CHR$(27) + "!" + CHR$(n)`)
- **TO CANCEL:** `ESC ! NUL` (`CHR$(27) + "!" + CHR$(0)`)

#### Table 6.3: Bit-Significant Values for `ESC ! <n>`

| Bit | Add Value | Attribute Selected |
| :---: | :---: | :--- |
| **Bit 0** | **0 / 1** | 0 = Standard (10 CPI / Pica) ; 1 = Mini (12 CPI / Elite) |
| **Bit 1** | **2** | Proportional typeface |
| **Bit 2** | **4** | Condensed typeface |
| **Bit 3** | **8** | Bold (Emphasized) |
| **Bit 4** | **16** | Double strike |
| **Bit 5** | **32** | Double width |
| **Bit 6** | **64** | Italics |
| **Bit 7** | **128** | Underline |

*Example:* To select Mini typeface (+1) with Double Strike (+16), set `<n>` = $1 + 16 = 17$:
```basic
LPRINT CHR$(27) + "!" + CHR$(17)
```
> **Note:** Do not set `<n>` to 9 as the printer will interpret this as a Horizontal Tab (`HT`).

---

### User defined characters (Download characters)

The DMP3000 allows users to design and download custom characters into printer RAM.

#### 4-Step Process:
1. **Download Character Definition:** `ESC & NUL <first> <last> <attribute> <d1> ... <d11>`
2. **Select Download Character Set:** `ESC % SOH NUL` (`CHR$(27) + "%" + CHR$(1) + CHR$(0)`)
3. **Enable Control Code Printing:** `ESC I SOH` (if defined in range 0–31)
4. **Print Character:** Send character code via `CHR$(n)`.

#### Attribute Byte Format:
- **Bit 7:** Descender flag (`0` = descender; `1` = no descender).
- **Bits 4–6:** Start column in 11-column matrix (0 to 7).
- **Bits 0–3:** End column in 11-column matrix (start + 4 to 11).

#### Copy Internal ROM Set to RAM:
```basic
LPRINT CHR$(27) + ":" + CHR$(0) + CHR$(0) + CHR$(0)
```

#### Complete Download Character Example Program (Square Box):
```basic
10 REM Download character definition (Box at code 5)
20 LPRINT CHR$(27) + "&" + CHR$(0) + CHR$(5) + CHR$(5) + CHR$(11);
30 FOR d=1 TO 11
40   READ n
50   LPRINT CHR$(n);
60 NEXT d
70 :
80 REM Select download character set
90 LPRINT CHR$(27) + "%" + CHR$(1) + CHR$(0)
100 :
110 REM Enable control code printing for code 5
120 LPRINT CHR$(27) + "I" + CHR$(1)
130 :
140 REM Print the custom character 40 times
150 FOR p=1 TO 40
160   LPRINT CHR$(5);
170 NEXT p
180 LPRINT
190 :
200 REM Data for square box (11 vertical columns)
210 DATA 127 : REM 1111111 binary
220 DATA 0   : REM 0000000 binary
230 DATA 65  : REM 1000001 binary
240 DATA 0   : REM 0000000 binary
250 DATA 65  : REM 1000001 binary
260 DATA 0   : REM 0000000 binary
270 DATA 65  : REM 1000001 binary
280 DATA 0   : REM 0000000 binary
290 DATA 65  : REM 1000001 binary
300 DATA 0   : REM 0000000 binary
310 DATA 127 : REM 1111111 binary
```
> **Note:** DIP switch **DS2-4 must be ON** to allocate printer RAM for download characters.

---

### Hexadecimal dump mode

Prints the exact hexadecimal value of every incoming byte received over the parallel interface:
1. Turn printer power **OFF**.
2. Hold down **both the LF and FF buttons** simultaneously while turning mains power **ON**.
3. The printer is now in Hex Dump Mode.
4. When printing stops, press **ON LINE** to switch off line and flush the final buffer contents.
5. Switch the printer **OFF** to exit hex dump mode.

# Chapter 7: For your reference...

### Subjects covered in this chapter:
- Technical Specification
- Printer socket
- Interface Pin Assignment (Centronics 36-Pin)
- DIP switch functions
- Signal timing

---

### Table 7.1: Technical Specification

| Parameter | DMP3160 | DMP3000 / DMP3250di |
| :--- | :--- | :--- |
| **Print System** | Impact dot-matrix | Impact dot-matrix |
| **Print Speed (Standard / Draft)** | **160 CPS** | **105 CPS** |
| **Print Speed (NLQ)** | **40 CPS** | **26 CPS** |
| **Printhead Configuration** | 9-wire vertical dot matrix | 9-wire vertical dot matrix |
| **Character Matrix** | 9 $\times$ 9 (Normal) / 9 $\times$ 10 (Double width) | 9 $\times$ 9 (Normal) / 9 $\times$ 10 (Double width) |
| **Bit Image Resolution** | 8 or 9 wires $\times$ chosen density | 8 or 9 wires $\times$ chosen density |
| **Character Sets** | 96 ASCII + Italics + 8 International sets + IBM Sets #1 & #2 | 96 ASCII + Italics + 8 International sets + IBM Sets #1 & #2 |
| **Character Size (Normal)** | 2.1 mm (W) $\times$ 2.55 mm (H) | 2.1 mm (W) $\times$ 2.55 mm (H) |
| **Pitch & Line Capacities** | | |
| - Standard (Pica) | 10 CPI / 80 CPL | 10 CPI / 80 CPL |
| - Mini (Elite) | 12 CPI / 96 CPL | 12 CPI / 96 CPL |
| - Condensed | 17 CPI / 137 CPL | 17 CPI / 137 CPL |
| - Double Width Standard | 5 CPI / 40 CPL | 5 CPI / 40 CPL |
| - Double Width Mini | 6 CPI / 48 CPL | 6 CPI / 48 CPL |
| - Double Width Condensed | 8.5 CPI / 68 CPL | 8.5 CPI / 68 CPL |
| **Line Feed Rates** | 1/6", 1/8", 7/72", $<n>$/216" programmable, $<n>$/72" programmable | 1/6", 1/8", 7/72", $<n>$/216" programmable, $<n>$/72" programmable |
| **Line Feed Speed (1/6")** | **160 ms** | **200 ms** |
| **Paper Handling** | Tractor feed & Friction feed | Tractor feed & Friction feed |
| - Fan-fold paper width | 4.5 to 10 inches | 4.5 to 10 inches |
| - Cut-sheet paper width | 4.0 to 9.5 inches | 4.0 to 9.5 inches |
| **Multi-Part Copies** | 2 sheets (including original), 40 g/m² pressure sensitive | 2 sheets (including original), 40 g/m² pressure sensitive |
| **Interface** | Standard 8-bit Parallel (Centronics compatible) | Standard 8-bit Parallel (Centronics compatible) |
| **Mains Supply** | 220–240 V AC, 50 Hz | 220–240 V AC, 50 Hz |
| **Dimensions** | 16" (W) $\times$ 10" (D) $\times$ 4" (H) (400 $\times$ 250 $\times$ 100 mm) | 16" (W) $\times$ 10" (D) $\times$ 4" (H) (400 $\times$ 250 $\times$ 100 mm) |
| **Weight** | 4.2 kg | 4.2 kg |

---

### Printer socket

The parallel interface connection is made via an industry-standard 36-pin female Amphenol/Centronics connector located on the rear panel of the printer, held by spring wire retaining latches.

---

### Table 7.2: 36-Pin Centronics Parallel Interface Pin Assignment

| Pin No. | Signal Designation | Direction | Return GND Pin | Functional Description |
| :---: | :--- | :---: | :---: | :--- |
| **1** | $\overline{\text{STROBE}}$ | IN | Pin 19 | Active low pulse. Taking pin low enables receiving of DATA 0 to DATA 7. Minimum necessary pulse width is $0.5\;\mu\text{s}$. |
| **2** | DATA 0 (LSB) | IN | Pin 20 | 8-bit parallel data signal. High = logical 1, Low = logical 0. |
| **3** | DATA 1 | IN | Pin 21 | Data bit 1. |
| **4** | DATA 2 | IN | Pin 22 | Data bit 2. |
| **5** | DATA 3 | IN | Pin 23 | Data bit 3. |
| **6** | DATA 4 | IN | Pin 24 | Data bit 4. |
| **7** | DATA 5 | IN | Pin 25 | Data bit 5. |
| **8** | DATA 6 | IN | Pin 26 | Data bit 6. |
| **9** | DATA 7 (MSB) | IN | Pin 27 | Data bit 7. |
| **10** | $\overline{\text{ACKNOWLEDGE}}$ | OUT | Pin 28 | Active low output pulse generated when data entry and processing are completed. Indicates printer is ready for next byte. Also generated when switching off line to on line. Pulse width approx $5\;\mu\text{s}$. |
| **11** | BUSY | OUT | Pin 29 | Output high when printer cannot receive data: a) During data entry/buffer full, b) During paper feed or printing, c) When off line, d) During printer error condition. |
| **12** | PE (Paper End) | OUT | Pin 30 | Output high when paper is out. (Sensed after paper feed when on line; always sensed when off line). |
| **13** | SELECT | OUT | — | Output high when printer is on line; low when off line. When off line, data cannot be received. |
| **14** | $\overline{\text{AFD}}$ (Auto Feed XT) | IN | — | Active low. Taking pin low automatically generates a line feed with each carriage return. |
| **15** | NC | — | — | No connection. |
| **16** | 0V | — | — | Logic ground (0V). |
| **17** | CHASSIS GND | — | — | Chassis / frame ground (isolated from signal ground). |
| **18** | +5V | OUT | — | +5V DC power supply output (50 mA maximum). |
| **19–30** | GND | — | — | Individual signal ground return lines for signals 1 through 12. |
| **31** | $\overline{\text{INPUT PRIME}}$ | IN | — | Active low. Taking pin low initialises and resets the printer. Minimum pulse width is $100\;\mu\text{s}$. |
| **32** | $\overline{\text{FAULT}}$ | OUT | — | Active low. Output low when printer is off line, out of paper, or in an error condition. |
| **33** | GND | — | — | Signal ground. |
| **34** | NC | — | — | No connection. |
| **35** | +5V | OUT | — | Logic high pull-up via resistor / +5V power rail. |
| **36** | $\overline{\text{SLCT IN}}$ | IN | — | Active low input. Taking pin low or high sets printer on line or off line respectively (when not in error). |

---

### Table 7.3: Complete DIP Switch Functions

> **REMEMBER:** Always switch the printer **OFF** before adjusting any DIP switches!

#### Bank 1: Switch Block DS1 (8 Switches)

| Switch | Function | OFF | ON | Default |
| :---: | :--- | :---: | :---: | :---: |
| **DS1-1** | International character set selection | See Chapter 2 / Table 2.2 | See Chapter 2 / Table 2.2 | **ON** (USA) |
| **DS1-2** | International character set selection | See Chapter 2 / Table 2.2 | See Chapter 2 / Table 2.2 | **ON** (USA) |
| **DS1-3** | International character set selection | See Chapter 2 / Table 2.2 | See Chapter 2 / Table 2.2 | **ON** (USA) |
| **DS1-4** | Carriage Return (`CR`) code definition | `CR` only | `CR` + `LF` (Auto Line Feed) | **OFF** |
| **DS1-5** | Paper-out sensor | Enable (Halts on paper out) | Disable (Ignores paper out) | **OFF** |
| **DS1-6** | Page length selection | 11 inches (66 lines) | 12 inches (72 lines) | **OFF** (11") |
| **DS1-7** | Default character set selection | See Table 2.1 | See Table 2.1 | **ON** |
| **DS1-8** | Default character set selection | See Table 2.1 | See Table 2.1 | **ON** (IBM #2) |

#### Bank 2: Switch Block DS2 (10 Switches)

| Switch | Function | OFF | ON | Default |
| :---: | :--- | :---: | :---: | :---: |
| **DS2-1** | Zero numeral character style | Unslashed zero ($0$) | Slashed zero ($\emptyset$) | **OFF** |
| **DS2-2** | Default skip perforation | Disable (Prints to bottom) | Enable (Leaves 1" top/bottom margin) | **OFF** |
| **DS2-3** | Buffer mode memory allocation | Character buffer (Standard) | Graphics buffer expanded | **OFF** |
| **DS2-4** | Buffer mode download allocation | Standard buffer | Download RAM enabled | **OFF** |
| **DS2-5** | $\overline{\text{SLCT IN}}$ interface signal | Not sent / Externally controlled | Automatically asserted | **ON** |
| **DS2-6** | Alarm bleeper | Disable audio buzzer | Enable audio buzzer | **ON** |
| **DS2-7** | Default power-on typeface | Bold off | Condensed & bold on | **OFF** |
| **DS2-8** | Default power-on typeface | Condensed off | Bold on | **OFF** |
| **DS2-9** | Reserved | Do not use (Factory-set) | Do not use (Factory-set) | **OFF** |
| **DS2-10**| Reserved | Do not use (Factory-set) | Do not use (Factory-set) | **OFF** |

---

### Signal timing

The Centronics interface handshaking operates under the following signal timing limits:

```
  DATA 0-7    ----< Valid Data >-----------------------------
                    |<- 0.5 µs min ->|
  STROBE      ---------\_________/---------------------------
                        |<- 0.5 µs ->|
  BUSY        --------------\___________________/------------
                             |<- 0.5 µs min ->|
  ACK         ---------------------------\______/------------
                                          |<- 5 µs ->|
```

- **Data Setup Time:** DATA 0–7 valid before $\overline{\text{STROBE}}$ falling edge $\ge 0.5\;\mu\text{s}$.
- **$\overline{\text{STROBE}}$ Pulse Width:** Minimum $0.5\;\mu\text{s}$.
- **Data Hold Time:** DATA 0–7 held after $\overline{\text{STROBE}}$ rising edge $\ge 0.5\;\mu\text{s}$.
- **BUSY Delay:** Rising edge of BUSY occurs within $0.5\;\mu\text{s}$ of $\overline{\text{STROBE}}$ falling edge.
- **$\overline{\text{ACKNOWLEDGE}}$ Pulse Width:** Approximately $5\;\mu\text{s}$ active low pulse upon completion of byte intake.

# Appendix 1: Table of Control Codes

The following master table lists all control codes recognised by the AMSTRAD DMP3000 / DMP3160 / DMP3250di, detailing the ASCII mnemonic, decimal code sequence, hexadecimal equivalent, and function.

| Code Mnemonic | Decimal Sequence | Hexadecimal Sequence | Function / Description | Chapter Ref |
| :--- | :--- | :--- | :--- | :---: |
| **BEL** | 7 | `&07` | Sound bleeper / bell | Ch 6 |
| **BS** | 8 | `&08` | Backspace one character position | Ch 4 |
| **HT** | 9 | `&09` | Horizontal tab jump to next preset tab stop | Ch 4 |
| **LF** | 10 | `&0A` | Line feed (advance paper 1 line & flush buffer) | Ch 4 |
| **VT** | 11 | `&0B` | Vertical tab jump to next preset vertical tab stop | Ch 4 |
| **FF** | 12 | `&0C` | Form feed (advance paper to top of next page) | Ch 4 |
| **CR** | 13 | `&0D` | Carriage return (return print head to left margin) | Ch 4 |
| **SO** | 14 | `&0E` | Select single-line double width printing | Ch 3 |
| **SI** | 15 | `&0F` | Select condensed typeface (17 CPI) | Ch 3 |
| **DC1** | 17 | `&11` | Device Control 1: Select printer (set on line) | Ch 6 |
| **DC2** | 18 | `&12` | Device Control 2: Cancel condensed typeface | Ch 3 |
| **DC3** | 19 | `&13` | Device Control 3: Deselect printer (set off line) | Ch 6 |
| **DC4** | 20 | `&14` | Device Control 4: Cancel single-line double width | Ch 3 |
| **CAN** | 24 | `&18` | Cancel / clear all data in the print buffer | Ch 6 |
| **DEL** | 127 | `&7F` | Delete previous character from print buffer | Ch 6 |
| **ESC SO** | 27 14 | `&1B &0E` | Select single-line double width printing | Ch 3 |
| **ESC SI** | 27 15 | `&1B &0F` | Select condensed typeface (17 CPI) | Ch 3 |
| **ESC ! `<n>`** | 27 33 `<n>` | `&1B &21 <n>` | Select master print mode (bit-significant font selection) | Ch 6 |
| **ESC #** | 27 35 | `&1B &23` | Accept 8th bit as sent from computer | Ch 6 |
| **ESC % `<n>` NUL** | 27 37 `<n>` 0 | `&1B &25 <n> &00` | Select internal (`<n>=0`) or download (`<n>=1`) character set | Ch 6 |
| **ESC & NUL `<params>`** | 27 38 0 `<n>...` | `&1B &26 &00 <n>...` | Define user-downloaded character into RAM | Ch 6 |
| **ESC \* `<params>`** | 27 42 `<m> <n1> <n2>...` | `&1B &2A <m> <n1> <n2>...`| Select bit image graphics mode (modes 0 to 6) | Ch 5 |
| **ESC - `<n>`** | 27 45 `<n>` | `&1B &2D <n>` | Select (`<n>=1`) or cancel (`<n>=0`) underline | Ch 3 |
| **ESC / `<n>`** | 27 47 `<n>` | `&1B &2F <n>` | Select vertical tab channel `<n>` (0 to 7) | Ch 4 |
| **ESC 0** | 27 48 | `&1B &30` | Select 1/8 inch paper feed pitch | Ch 4 |
| **ESC 1** | 27 49 | `&1B &31` | Select 7/72 inch paper feed pitch | Ch 4 |
| **ESC 2** | 27 50 | `&1B &32` | Select 1/6 inch paper feed pitch (standard default) | Ch 4 |
| **ESC 3 `<n>`** | 27 51 `<n>` | `&1B &33 <n>` | Select variable `<n>`/216 inch paper feed | Ch 4 |
| **ESC 4** | 27 52 | `&1B &34` | Select italics option | Ch 3 |
| **ESC 5** | 27 53 | `&1B &35` | Cancel italics option | Ch 3 |
| **ESC 6** | 27 54 | `&1B &36` | Select printable code area expansion (128–159 & 255) | Ch 6 |
| **ESC 7** | 27 55 | `&1B &37` | Cancel printable code area expansion | Ch 6 |
| **ESC 8** | 27 56 | `&1B &38` | Disable paper out sensor detection | Ch 6 |
| **ESC 9** | 27 57 | `&1B &39` | Enable paper out sensor detection | Ch 6 |
| **ESC : NUL `<params>`** | 27 58 0 0 0 | `&1B &3A &00 &00 &00` | Copy internal ROM character set into download RAM | Ch 6 |
| **ESC <** | 27 60 | `&1B &3C` | Home head (one-line unidirectional print) | Ch 6 |
| **ESC =** | 27 61 | `&1B &3D` | Unset eighth bit (force bit 7 to 0) | Ch 6 |
| **ESC >** | 27 62 | `&1B &3E` | Set eighth bit (force bit 7 to 1) | Ch 6 |
| **ESC ? `<params>`** | 27 63 `<code> <m>` | `&1B &3F <code> <m>` | Re-assign graphics escape code letter to mode `<m>` | Ch 5 |
| **ESC @** | 27 64 | `&1B &40` | Reset printer to power-on initialization defaults | Ch 6 |
| **ESC A `<n>`** | 27 65 `<n>` | `&1B &41 <n>` | Select variable `<n>`/72 inch paper feed pitch | Ch 4 |
| **ESC B `<params>` NUL** | 27 66 `<n>...` 0 | `&1B &42 <n>... &00` | Set vertical tab stops (up to 16, terminated by NUL) | Ch 4 |
| **ESC C `<n>`** | 27 67 `<n>` | `&1B &43 <n>` | Set page length by lines (`<n>` = 1 to 127) | Ch 4 |
| **ESC C NUL `<n>`** | 27 67 0 `<n>` | `&1B &43 &00 <n>` | Set page length by inches (`<n>` = 1 to 22) | Ch 4 |
| **ESC D `<params>` NUL** | 27 68 `<n>...` 0 | `&1B &44 <n>... &00` | Set horizontal tab stops (up to 32, terminated by NUL) | Ch 4 |
| **ESC E** | 27 69 | `&1B &45` | Select bold (emphasized) option | Ch 3 |
| **ESC F** | 27 70 | `&1B &46` | Cancel bold (emphasized) option | Ch 3 |
| **ESC G** | 27 71 | `&1B &47` | Select double strike option | Ch 3 |
| **ESC H** | 27 72 | `&1B &48` | Cancel double strike option | Ch 3 |
| **ESC I `<n>`** | 27 73 `<n>` | `&1B &49 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) control code printing | Ch 6 |
| **ESC J `<n>`** | 27 74 `<n>` | `&1B &4A <n>` | Variable `<n>`/216 inch one-shot forward feed | Ch 4 |
| **ESC K `<n1> <n2>`** | 27 75 `<n1> <n2>` | `&1B &4B <n1> <n2>` | Select single density bit image graphics (480 dots/line) | Ch 5 |
| **ESC L `<n1> <n2>`** | 27 76 `<n1> <n2>` | `&1B &4C <n1> <n2>` | Select double density bit image graphics (960 dots/line) | Ch 5 |
| **ESC M** | 27 77 | `&1B &4D` | Select mini typeface (12 CPI / Elite) | Ch 3 |
| **ESC N `<n>`** | 27 78 `<n>` | `&1B &4E <n>` | Select skip perforation (`<n>` = 1 to 127 lines) | Ch 4 |
| **ESC O** | 27 79 | `&1B &4F` | Cancel skip perforation | Ch 4 |
| **ESC P** | 27 80 | `&1B &50` | Cancel mini typeface (return to standard Pica) | Ch 3 |
| **ESC Q `<n>`** | 27 81 `<n>` | `&1B &51 <n>` | Set right margin at column `<n>` (1 to 255) | Ch 4 |
| **ESC R `<n>`** | 27 82 `<n>` | `&1B &52 <n>` | Select international character set (0 to 8) | Ch 6 |
| **ESC S `<n>`** | 27 83 `<n>` | `&1B &53 <n>` | Select subscript (`<n>=1`) or superscript (`<n>=0`) | Ch 3 |
| **ESC T** | 27 84 | `&1B &54` | Cancel subscript and superscript options | Ch 3 |
| **ESC U `<n>`** | 27 85 `<n>` | `&1B &55 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) uni-directional printing | Ch 6 |
| **ESC W `<n>`** | 27 87 `<n>` | `&1B &57 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) continuous double-width | Ch 3 |
| **ESC Y `<n1> <n2>`** | 27 89 `<n1> <n2>` | `&1B &59 <n1> <n2>` | Select double speed double density graphics (960 dots/line)| Ch 5 |
| **ESC Z `<n1> <n2>`** | 27 90 `<n1> <n2>` | `&1B &5A <n1> <n2>` | Select quadruple density graphics (1920 dots/line) | Ch 5 |
| **ESC ^ `<m> <n1> <n2>`**| 27 94 `<m> <n1> <n2>` | `&1B &5E <m> <n1> <n2>` | Select 9-pin bit image graphics mode | Ch 5 |
| **ESC b `<params>` NUL** | 27 98 `<c> <n>...` 0 | `&1B &62 <c> <n>... &00` | Set vertical tabs in channel `<c>` | Ch 4 |
| **ESC i `<n>`** | 27 105 `<n>` | `&1B &69 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) incremental printing | Ch 6 |
| **ESC j `<n>`** | 27 106 `<n>` | `&1B &6A <n>` | Variable `<n>`/216 inch one-shot reverse paper feed | Ch 4 |
| **ESC l `<n>`** | 27 108 `<n>` | `&1B &6C <n>` | Set left margin at column `<n>` (0 to 255) | Ch 4 |
| **ESC m `<n>`** | 27 109 `<n>` | `&1B &6D <n>` | Select character table (0 = Epson FX; 1 = IBM #1; 2 = IBM #2)| Ch 6 |
| **ESC p `<n>`** | 27 112 `<n>` | `&1B &70 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) proportional typeface | Ch 3 |
| **ESC s `<n>`** | 27 115 `<n>` | `&1B &73 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) half speed printing | Ch 6 |
| **ESC x `<n>`** | 27 120 `<n>` | `&1B &78 <n>` | Select (`<n>=1`) or cancel (`<n>=0`) Near Letter Quality (NLQ)| Ch 3 |

# Appendix 2: Character Tables

The character tables available on the AMSTRAD DMP3000 / DMP3160 / DMP3250di are selectable via DIP switches **DS1-7** and **DS1-8** or software escape code `ESC m <n>`.

---

### Character Set Summary

- **Table 1:** Epson FX - Standard (DS1-7 OFF, DS1-8 OFF | `ESC m 0`)
- **Table 2:** Epson FX - NLQ (DS1-7 ON, DS1-8 OFF | `ESC m 0` + `ESC x 1`)
- **Table 3.1:** IBM Character Set #1 - Standard (DS1-7 OFF, DS1-8 ON | `ESC m 1`)
- **Table 3.2:** IBM Character Set #2 - Standard (DS1-7 ON, DS1-8 ON | `ESC m 2` — *Factory Default*)
- **Table 4.1:** IBM Character Set #1 - NLQ (DS1-7 OFF, DS1-8 ON + NLQ)
- **Table 4.2:** IBM Character Set #2 - NLQ (DS1-7 ON, DS1-8 ON + NLQ)

---

### Table 1: Epson FX - Standard Character Set (0 to 255)

- **Codes 0 to 31 (`&00` to `&1F`):** Standard ASCII printer control codes (see Appendix 1).
- **Codes 32 to 126 (`&20` to `&7E`):** Standard printable ASCII characters:
  - 32: *(Space)* | 33–47: `! " # $ % & ' ( ) * + , - . /`
  - 48–57: Numerals `0 1 2 3 4 5 6 7 8 9`
  - 58–64: `: ; < = > ? @`
  - 65–90: Uppercase `A B C D E F G H I J K L M N O P Q R S T U V W X Y Z`
  - 91–96: `[ \ ] ^ _ \``
  - 97–122: Lowercase `a b c d e f g h i j k l m n o p q r s t u v w x y z`
  - 123–126: `{ | } ~`
- **Code 127 (`&7F`):** Delete (`DEL`).
- **Codes 128 to 159 (`&80` to `&9F`):** Control codes or printable symbols when expanded via `ESC 6`.
- **Codes 160 to 254 (`&A0` to `&FE`):** Italic font representations of characters 32 to 126.
- **Code 255 (`&FF`):** Delete / blank (or graphics symbol via `ESC 6`).

---

### Table 2: Epson FX - NLQ Character Set

Reproduces the complete character set of Table 1 using high-density **Near Letter Quality (NLQ)** dot patterns. Characters have enhanced stroke weight, rounded curves, and eliminated dot gaps for typewriter-quality appearance.

---

### Table 3.1: IBM Character Set #1 (Standard)

- **Codes 0 to 31:** Control codes.
- **Codes 32 to 127:** Standard printable ASCII characters.
- **Codes 128 to 159:** Repetition of control codes 0 to 31 (unless `ESC 6` is active).
- **Codes 160 to 255:** Extended character set featuring European accented vowels, mathematical signs, block graphics, and single/double border lines:
  - `á, í, ó, ú, ñ, Ñ, ª, º, ¿, ⌐, ¬, ½, ¼, ¡, «, »`
  - Box drawing symbols: `░, ▒, ▓, │, ┤, ╡, ╢, ╖, ╕, ╣, ║, ╗, ╝, ╜, ╛, ┐, └, ┴, ┬, ├, ─, ┼, ╞, ╟, ╚, ╔, ╩, ╦, ╠, ═, ╬`
  - Greek letters & math symbols: `α, β, Γ, π, Σ, σ, µ, τ, Φ, Θ, Ω, δ, ∞, φ, ε, ∩, ≡, ±, ≥, ≤, ⌠, ⌡, ÷, ≈, °, ∙, ·, √, ⁿ, ², ■`

---

### Table 3.2: IBM Character Set #2 (Standard — Factory Default)

In IBM Character Set #2, the code areas **0 to 31** and **128 to 159** are populated with printable characters and symbols:
- **Codes 1 to 31:** Printable symbols (such as smiling faces, playing card suits: hearts, diamonds, clubs, spades, musical notes, arrows: `☺, ☻, ♥, ♦, ♣, ♠, •, ◘, ○, ◙, ♂, ♀, ♪, ♫, ☼, ►, ◄, ↕, ‼, ¶, §, ▬, ↨, ↑, ↓, →, ←, ∟, ↔, ▲, ▼`).
- **Codes 128 to 159:** International European accented characters:
  - `128: Ç`, `129: ü`, `130: é`, `131: â`, `132: ä`, `133: à`, `134: å`, `135: ç`
  - `136: ê`, `137: ë`, `138: è`, `139: ï`, `140: î`, `141: ì`, `142: Ä`, `143: Å`
  - `144: É`, `145: æ`, `146: Æ`, `147: ô`, `148: ö`, `149: ò`, `150: û`, `151: ù`
  - `152: ÿ`, `153: Ö`, `154: Ü`, `155: ø`, `156: £`, `157: Ø`, `158: ×`, `159: ƒ`
- **Codes 160 to 255:** Identical to IBM Extended character set (box graphics, Greek, math).

---

### Table 4.1 & Table 4.2: IBM Character Sets #1 & #2 (NLQ)

Tables 4.1 and 4.2 reproduce IBM Character Sets #1 and #2 respectively in high-density **Near Letter Quality** typeface.

> **IMPORTANT NOTE ON INTERNATIONAL CHARACTERS:**  
> The printing of international characters using DIP switches DS1-1, DS1-2, DS1-3 or escape code `ESC R <n>` is **NOT possible** while IBM character set #1 or #2 is in use. To print international character substitutions, ensure that DIP switch **DS1-8 is OFF** or select `ESC m 0`.

# Appendix 3: Index

An alphabetical index of subjects and their corresponding page numbers in the printed manual:

| Subject | Original Manual Page |
| :--- | :--- |
| **Alarm bleeper** | 14, 31, 62, 74 |
| **Alignment of characters** | 56 |
| **AMSTRAD computers** | 1, 13 |
| **ASCII** | 17, 29, 32, 70 |
| **Backspace** | 44 |
| **BASIC** | 18, 21, 26 |
| **BBC Microcomputer** | 1, 13 |
| **Bell** | 62 |
| **Bit image graphics** | 51, 54, 55, 56 |
| **Bleeper** | 14, 31, 62, 74 |
| **Bold printing** | 38, 65 |
| **Buffer (print buffer)** | 26, 27 |
| **Button controls (LF, FF, ON LINE)** | 14, 16, 17, 18 |
| **Cancelling print styles** | 34, 35, 36, 37, 38 |
| **Carriage return** | 43, 74 |
| **Centronics interface** | 12, 73 |
| **Changing typeface** | 31, 65 |
| **Character alignment** | 56 |
| **Character set** | 27, 58, 63, 83 |
| **Character tables** | 27, 58, 63, 83 |
| **Commodore computer** | 1, 13 |
| **Compatibility** | 1 |
| **Condensed typeface** | 35, 65 |
| **Connecting to a home computer** | 13 |
| **Connecting to a PC** | 12 |
| **Connecting to mains supply** | 14 |
| **Control codes** | 31, 77 |
| **Control layout** | 14 |
| **CP/M files** | 26 |
| **Deleting characters** | 62 |
| **Denmark** | 30, 64 |
| **Descender** | 67 |
| **DIP switches** | 28, 29, 74 |
| **Disk directory** | 24 |
| **DOS files** | 23 |
| **DOS Plus** | 26 |
| **Double density graphics** | 54 |
| **Double speed double density graphics** | 54 |
| **Double strike printing** | 37, 65 |
| **Double width printing** | 38, 65 |
| **Download characters** | 66, 67, 68, 69 |
| **Dump (hexadecimal)** | 70 |
| **Echoing screen output to printer** | 25 |
| **Eighth bit setting** | 60 |
| **Elite typeface (Mini)** | 34, 65 |
| **Epson character set** | 28, 83 |
| **Escape code (`ESC`)** | 31, 32 |
| **FF button** | 17, 18 |
| **Filenames** | 22, 24 |
| **Flushing the buffer** | 26, 27 |
| **Foreign characters** | 29, 58, 64 |
| **Form feed** | 17, 18, 44 |
| **France** | 30, 64 |
| **Friction/tractor switch** | 14, 15, 19 |
| **GEM files** | 25 |
| **Germany** | 30, 64 |
| **Graphics character set** | 27, 29, 56, 87, 89, 91, 93 |
| **Graphics printing** | 51, 56, 74 |
| **Half speed printing** | 63 |
| **Hexadecimal dump** | 70 |
| **Home head** | 62 |
| **Horizontal tab** | 46, 47 |
| **IBM BASIC** | 18 |
| **IBM character set** | 27, 83 |
| **IBM PC** | 1, 12, 13 |
| **Incremental printing** | 58 |
| **Ink ribbon** | 8, 9 |
| **Interface** | 12, 73 |
| **Internal character set** | 27, 68 |
| **International characters** | 29, 64 |
| **Italics printing** | 37 |
| **Italy** | 30, 64 |
| **Japan** | 64 |
| **Layout of controls** | 14 |
| **Left margin** | 44 |
| **Legs** | 19, 20 |
| **LF button** | 16, 18 |
| **Line feed** | 16, 43, 74 |
| **Listing a program** | 21 |
| **Listing the disk directory** | 24, 25 |
| **Loading paper** | 15, 19 |
| **Locomotive BASIC 2** | 18 |
| **Mains ON/OFF switch** | 14 |
| **Mains plug wiring** | 6 |
| **Maintenance** | 2 |
| **Manual paper feed knob** | 14 |
| **Margins** | 44, 45 |
| **Microsoft BASIC** | 18 |
| **Mini typeface (Elite)** | 34, 65 |
| **Moving the paper** | 16, 17 |
| **MS-DOS** | 22 |
| **NLQ-proportional typeface** | 36 |
| **NLQ-standard typeface** | 31, 35 |
| **Notation** | 22 |
| **NUL code** | 22, 32 |
| **Off line** | 17, 18 |
| **On line** | 17, 18 |
| **ON LINE button & lamp** | 14, 17 |
| **Page length** | 45 |
| **Paper feed knob** | 14 |
| **Paper feed rates** | 48, 49, 50 |
| **Paper guide bar** | 7, 20 |
| **Paper guides** | 15 |
| **Paper loading** | 15, 19 |
| **Paper out sensor** | 14, 61, 74 |
| **Paper thickness lever** | 16 |
| **Parallel interface** | 12, 73 |
| **PC / PC-DOS** | 1, 12, 22 |
| **Perforation skip** | 45, 46 |
| **Pica typeface (Standard)** | 34, 65 |
| **PL-1 printer lead (Centronics to DB-25)**| 12 |
| **PL-2 printer lead (Centronics to CPC)**  | 13 |
| **Power lamp** | 14 |
| **Preparation** | 7 |
| **Printable code area expansion** | 59 |
| **Print buffer** | 26, 27 |
| **Printer cover** | 15, 16, 19 |
| **Printer echo** | 25 |
| **Printer lead** | 12, 13 |
| **Printer legs** | 19, 20 |
| **Printer socket** | 12, 72, 73 |
| **Print formatting** | 43–50 |
| **Print head stabilisers** | 7 |
| **Printing** | 18, 21 |
| **Print mode (`ESC !`)** | 65 |
| **Proportional attributes** | 67 |
| **Proportional typeface** | 35 |
| **Quadruple density graphics** | 54 |
| **Reset printer (`ESC @`)** | 61 |
| **Reverse paper feed** | 50 |
| **Ribbon fitting** | 8, 9 |
| **Right margin** | 45 |
| **Screen dump** | 25 |
| **Self test printing** | 17 |
| **Servicing** | 2 |
| **Signal timing** | 75 |
| **Sinclair computer** | 1, 13 |
| **Single density graphics** | 54 |
| **Skip perforation** | 45, 46 |
| **Slashed zero** | 74 |
| **SOH code** | 22, 32 |
| **Spain** | 30, 64 |
| **Specification** | 71, 72 |
| **Standard typeface** | 34, 65 |
| **Subscript printing** | 36, 40 |
| **Superscript printing** | 37, 40 |
| **Sweden** | 30, 64 |
| **Switching on** | 14 |
| **Tab channels** | 48 |
| **Tabulation** | 46, 47 |
| **Technical specification** | 71, 72 |
| **Thickness of paper** | 16 |
| **Timing** | 75 |
| **Tractor feed paper** | 15, 19 |
| **Tractors** | 15, 19 |
| **Typefaces** | 33–36 |
| **Typewriter comparison** | 1, 31 |
| **Underlining** | 38 |
| **Uni-directional printing** | 56, 62 |
| **Unpacking** | 7 |
| **User defined characters** | 66–69 |
| **Variable paper feed** | 49, 50 |
| **Vertical tab** | 47, 48 |
| **WIDTH command** | Addendum, 51 |
| **Wildcards** | 23, 25 |
| **Zero slashing** | 74 |

---

### End of Manual

**AMSTRAD DMP3000 / DMP3160 / DMP3250di User Manual**  
Part Number: **802D0201-3251**  
First Published: 1986 | Second Edition: 1987  
Published by AMSTRAD Plc., Brentwood, Essex, England.
