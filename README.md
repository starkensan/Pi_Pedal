# USB-MIDI-Pedal

RP2040 Zero を使用して、ギター用フットスイッチやエクスプレッションペダルから  
USB-MIDI（CC / PC）メッセージを送信できるデバイスです。  
DAW ソフトやアンプシミュレーター（Amplitube / Helix Native など）で使用できます。

## Features
- USB MIDI（CC / PC）送信対応
- フットスイッチによる CC/PC 切り替え
- エクスプレッションペダル対応（アナログ入力）
- OLED による状態表示
- ノブによる直感的な操作
- ラッチ / モーメンタリ切り替え
- デバウンス処理付き
- コンパクトな RP2040 Zero ベース
- PCB 設計データ & 3D モデル付き

---

## Product image

![Prodct_image_1](/image/image1.jpg)
![Prodct_image_2](/image/image2.jpg)


---

## Hardware
- **Microcontroller:** RP2040 Zero  
- **Switches:** 1~（自由に拡張可能）  
- **Expression Pedal:** TRS（0〜3.3V）  
- **Display:** 0.96" OLED (I2C)
- **Rotary Encoder** EC11
- **ADC IC** MCP3421
- **その他:** 6.35mm ジャック、TRS、PCB（KiCad）、3Dプリントケース

PCB と 3D モデルは `/hardware/pcb/` と `/hardware/3d/` に配置しています。

---

## 🔧 Software
- **Language:** C++ / Arduino Core RP2040  
- **USB MIDI:** TinyUSB  
- **スイッチ管理:** デバウンス、ラッチ/モーメンタリ対応  
- **表示:** Adafruit SSD1306@^2.5.15

---

## 🚀 Setup / Installation

### 1. Clone this repository
```bash
git clone https://github.com/starkensan/Pi_Pedal.git