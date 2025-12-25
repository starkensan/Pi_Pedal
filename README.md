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


## Product image

![Prodct_image_1](/image/image1.jpg)
![Prodct_image_2](/image/image2.jpg)



## Hardware
- **Microcontroller:** RP2040 Zero  
- **Switches:** 1~（自由に拡張可能）  
- **Expression Pedal:** TRS（0〜3.3V）  
- **Display:** 0.96" OLED (I2C)
- **Rotary Encoder** EC11
- **ADC IC** MCP3421
- **その他:** 6.35mm ジャック、TRS、PCB（KiCad）、3Dプリントケース

PCB と 3D モデルは `/hardware/pcb/` と `/hardware/3d/` に配置しています。


## Software
- **Language:** C++ / Arduino Core RP2040  
- **USB MIDI:** TinyUSB  
- **スイッチ管理:** デバウンス、ラッチ/モーメンタリ対応  
- **表示:** Adafruit SSD1306@^2.5.15


## Build & Upload (CUI / PlatformIO CLI)

本ドキュメントでは、VS Code を使用せず、  
**CUI（ターミナル）から PlatformIO CLI を用いて**  
ビルドおよび RP2040 Zero への書き込みを行う手順を説明します。


### 事前準備

- Python 3.x
- PlatformIO Core（CLI）

#### PlatformIO CLI のインストール

```bash
pip install platformio
````

インストール確認：

```bash
pio --version
```


### ビルド（env:pico）

本プロジェクトでは、通常動作用に `env:pico` を使用します。

```bash
pio run -e pico
```

ビルドが成功すると、以下のディレクトリに成果物が生成されます。

```
.pio/build/pico/
```


### 書き込み（USB / picotool）

RP2040 Zero を USB 接続した状態で、以下を実行します。

```bash
pio run -e pico -t upload
```


### 書き込みに失敗する場合（BOOTSEL モード）

1. RP2040 Zero の **BOOT** ボタンを押しながら USB 接続
2. PC に `RPI-RP2` ドライブが表示されることを確認
3. 再度アップロードを実行

```bash
pio run -e pico -t upload
```

### UF2 ファイルを手動で書き込む方法

PlatformIO による自動書き込みができない場合は、
生成された UF2 ファイルを直接コピーすることで書き込み可能です。

UF2 ファイルの場所：

```
.pio/build/pico/firmware.uf2
```

この `firmware.uf2` を `RPI-RP2` ドライブへ
ドラッグ＆ドロップしてください。


### シリアルモニタ（ログ確認）

```bash
pio device monitor -e pico
```

※ ボーレートは `platformio.ini` にて `115200` に設定されています。


### 接続デバイス一覧の確認

```bash
pio device list
```


### 補足：env の使い分け

* `env:pico`
  通常動作用（USB MIDI / picotool）

* `env:pico-debug`
  CMSIS-DAP を使用したデバッグ用
