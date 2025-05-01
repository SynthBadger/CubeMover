
# 🦾 Robotic Arm + Voice Control System

This project integrates a **MAX robotic arm**, **vacuum nozzle system**, **Waveshare barcode scanner**, and a **PC-based voice control interface**, all coordinated via an **ESP32 microcontroller**.

---

## 📁 Project Structure

```
/ProjectFolder
├── CubeSniffer_copy_20250501125414.ino   # Main Arduino sketch for ESP32
├── _espmax.cpp / .h                      # MAX Arm + integrated nozzle control
├── ESPMax.cpp / .h
├── LobotSerialServoControl.cpp / .h      # Servo control
├── SuctionNozzle.cpp / .h                # Internal pump/valve control logic
└── Voice Control/                        # PC-side voice control system
    ├── voice_control.py
    ├── voice_gui.py
    ├── VoiceCommandInstaller.exe
    ├── VoiceControl.iss
```

---

## 🔧 Hardware Requirements

- ✅ **ESP32 Dev Board**
- ✅ **MAX Robotic Arm with integrated suction nozzle**
- ✅ **Waveshare Barcode Scanner Module (1D/2D QR Code Reader)**  
  📎 [Barcode Scanner Setup Manual (PDF)](https://files.waveshare.com/wiki/Barcode-Scanner-Module/Barcode_Scanner_Module_Setting_Manual_V2.1.pdf)

---

### 🧪 Barcode Scanner Setup

The Waveshare Barcode Scanner connects via **USB** (not UART) and must be configured properly.

#### ⚙ Required Settings:

1. Open the setup manual:  
   📄 [Setting Manual V2.1](https://files.waveshare.com/wiki/Barcode-Scanner-Module/Barcode_Scanner_Module_Setting_Manual_V2.1.pdf)

2. Scan these barcodes from the manual:
   - ✅ **Set USB HID Device Mode**
   - ✅ **Enable Continuous Mode**

3. Connect the scanner to the ESP32 system using a USB interface or USB Host Shield.

---

## 🚀 How to Use

### 1️⃣ Upload the Arduino Code to ESP32

1. Open `CubeSniffer_copy_20250501125414.ino` in Arduino IDE.
2. Ensure all `.cpp` and `.h` files are in the same directory.
3. Select your ESP32 board and upload.

---

### 2️⃣ Install and Run Voice Control App (on PC)

1. Go to the `Voice Control` folder.
2. Run `VoiceCommandInstaller.exe` to install the voice interface.

🛠 Or run manually (if using source):
```bash
pip install pyserial speechrecognition pyttsx3
python voice_gui.py
```

---

### 3️⃣ Use Voice Commands

Examples:
- `"find red cube in section 1"` → Moves arm, scans barcode, picks up match
- `"go home"` → Returns to base position
- `"pump on"` / `"valve"` → Controls suction nozzle (integrated into arm)

---

## 🎮 Additional Features

- 🕹 **Manual Control** using:
  - `WASD` for X/Y
  - `Q/E` for Z
  - Direct `X Y Z` input
- 📦 **Automated Barcode Scanning**
- 🧠 **Voice Recognition Interface**
- 🔄 **Integrated Suction Nozzle Control** (pump & valve)

---

## 🧩 Dependencies

### Arduino IDE:
- ESP32 board support
- Servo & serial libraries

### Python (PC):
- `pyserial`
- `speechrecognition`
- `pyttsx3` *(optional for text-to-speech)*
- `tkinter` *(built-in GUI)*
