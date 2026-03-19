# TokenDisplayEsp32
Token Display 

Full wiring diagrams and instructions ready for your GitHub README.md.

Copy-paste this into README.md. Includes Fritzing-style text diagrams + tables.

ESP32 Token Display with Keypad, 7-Segment & Voice
Queue token system: Enter number on keypad → wireless to display + voice announcement.

Components
| Item                 | Qty | Notes                |
| -------------------- | --- | -------------------- |
| ESP32 DevKit         | 2   | 30-pin               |
| 4x4 Keypad           | 1   | Membrane             |
| TM1637 4-digit 7-seg | 2   | Common cathode       |
| DFPlayer Mini        | 1   | + microSD + speaker  |
| Speaker              | 1   | 3W 8Ω                |
| Jumper wires         | -   | -                    |
| Breadboard           | 2   | -                    |
| 5V PSU               | 1   | For DFPlayer/speaker |

Sender Wiring (Keypad ESP32)
ESP32          4x4 Keypad     TM1637
GPIO13 ─────── Row1
GPIO12 ─────── Row2           CLK ─ GPIO18
GPIO14 ─────── Row3           DIO ─ GPIO19
GPIO27 ─────── Row4
GPIO26 ─────── Col1
GPIO25 ─────── Col2
GPIO33 ─────── Col3
GPIO32 ─────── Col4
GND ──────── GND ──────── GND
3V3 ───────── VCC ───────── VCC (or 5V)
VIN(5V) ─────── (if keypad 5V)

Keypad Pinout Table:
| Keypad   | ESP32 GPIO |
| -------- | ---------- |
| 1 (Row1) | 13         |
| 2 (Row2) | 12         |
| 3 (Row3) | 14         |
| 4 (Row4) | 27         |
| 5 (Col1) | 26         |
| 6 (Col2) | 25         |
| 7 (Col3) | 33         |
| 8 (Col4) | 32         |

Receiver Wiring (Display ESP32)
ESP32          TM1637        DFPlayer Mini     Speaker
GPIO18 ─────── CLK
GPIO19 ─────── DIO           RX  ─ GPIO17(TX)
GND ──────── GND ──────── GND ─────── SPK-
VIN(5V) ───── VCC           VCC ─────── SPK+
                    TX  ─ GPIO16(RX)

DFPlayer Pinout:

| DFPlayer | ESP32       |
| -------- | ----------- |
| VCC      | VIN (5V)    |
| GND      | GND         |
| RX       | GPIO17 (TX) |
| TX       | GPIO16 (RX) |

Software Setup
1. Arduino IDE + ESP32 board (2.0.17+)
2. Libraries: Keypad, TM1637Display, DFRobotDFPlayerMini
3. Get MACs: Flash one, open Serial Monitor → WiFi.macAddress()
4. Update MACs in codes
5. SD Card (FAT32): /01/0001.mp3 ("Token zero zero zero one"), etc.

Usage
Keypad: 1-2-3-4 → # (send/clear)
Display: Shows token + plays voice
Range: 200m LOS

Schematic (ASCII)
[Keypad ESP32] ─ESP-NOW───> [Display ESP32]
     |                          |
  7-seg Display             7-seg + DFPlayer → Speaker
