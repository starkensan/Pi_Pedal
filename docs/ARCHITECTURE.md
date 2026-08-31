# ソフトウェアアーキテクチャ

このドキュメントは、USB-MIDI Pedal ファームウェアの主要コンポーネント、データの流れ、メニュー操作の状態遷移を説明します。

## 目的と実行環境

RP2040 Zero 上で動作し、フットスイッチとエクスプレッションペダルの入力を USB-MIDI の CC（Control Change）または PC（Program Change）メッセージに変換します。OLED とロータリーエンコーダを使い、各ペダルの設定を本体だけで変更できます。

- 言語／フレームワーク: C++、Arduino Core for RP2040、PlatformIO
- USB MIDI: TinyUSB と MIDI Library
- 設定保存先: EEPROM 互換ストレージ
- 表示: I2C 接続 OLED

## 全体構成

```mermaid
classDiagram
    direction LR

    class Main {
        +setup()
        +loop()
        +setup1()
        +loop1()
    }
    class PedalsController {
        +begin(deviceName)
        +start()
        +update()
        +attachPCCallbacks(callback)
    }
    class MenuController {
        +begin()
        +update()
        +showPCNumber(number)
    }
    class MenuManager {
        +init(menuID)
        +enterSelectedItem()
        +cusorUp()
        +cusorDown()
    }
    class MenuDisplay {
        +render()
        +renderNumber()
    }
    class SettingsManager {
        +begin()
        +getPedalSettings()
        +commitSettings()
        +FactoryReset()
    }
    class Pedal
    class ExpPedal
    class RotaryEncoder
    class DrawDisplay
    class USBMIDI
    class EepromStorage

    Main --> PedalsController
    Main --> MenuController
    PedalsController --> Pedal : foot switch input
    PedalsController --> ExpPedal : expression input
    PedalsController --> USBMIDI : sends CC / PC
    PedalsController --> SettingsManager : reads settings
    MenuController --> RotaryEncoder : receives operation
    MenuController --> MenuManager
    MenuManager --> MenuDisplay
    MenuDisplay --> DrawDisplay : renders OLED
    MenuManager --> SettingsManager : edits settings
    SettingsManager --> EepromStorage : persists settings
```

`Hal*` インターフェース（`HalPedal`、`HalExpPedal`、`HalUSBMIDI`、`HalDisplay`、`HalRotaryEncoder`、`HalStorage`）が、アプリケーション層と具体的なハードウェア実装の間を分離します。このため、nativeテストでは実機の代わりにモックを渡せます。

## 二つの実行ループ

RP2040 の二つのコアを使用し、演奏時のMIDI処理と画面／設定操作を分離します。

```mermaid
flowchart TB
    subgraph Core0[Core 0: MIDI処理]
        A[setup] --> B[SettingsManager.begin]
        B --> C[PedalsController.begin / start]
        C --> D[loop]
        D --> E[PedalsController.update]
        E --> F[フットスイッチ・EXPペダルを更新]
        F --> D
    end

    subgraph Core1[Core 1: UI処理]
        G[setup1] --> H[SettingsManager.begin]
        H --> I[MenuController.begin]
        I --> J[loop1]
        J --> K[PC番号通知があればOLEDへ反映]
        K --> L[ロータリーエンコーダを更新]
        L --> J
    end

    F -. PC番号コールバック .-> K
```

`PedalsController` が PC を送信すると、登録済みコールバックを介して `main.cpp` のPC番号を更新します。Core 1 側はその値を `MenuController` に渡し、メイン画面へ表示します。

## 演奏時の処理

```mermaid
flowchart LR
    Input[入力変化] --> Type{入力種別}
    Type -->|フットスイッチ| Mode{設定されたモード}
    Type -->|EXPペダル| Exp[値を0〜127へ制限]
    Mode -->|CC / Momentary| CC1[押下=127・解放=0のCC]
    Mode -->|CC / Toggle| Toggle[押下ごとにON/OFFを反転してCC]
    Mode -->|PC Next| Next[PC番号を加算し0〜127で循環]
    Mode -->|PC Back| Back[PC番号を減算し0〜127で循環]
    CC1 --> MIDI[USB MIDI送信]
    Toggle --> MIDI
    Next --> MIDI
    Back --> MIDI
    Exp --> MIDI
```

- フットスイッチのデバウンスは `Pedal` が担当します。
- `PedalsController` は `SettingsManager` から各ペダルのモード、MIDIチャンネル、CC番号、スイッチ方式を読み取ります。
- EXPペダルは MCP3421 で読み取り、変化量がしきい値を超えた場合だけ CC を送信します。

## 設定のライフサイクル

```mermaid
stateDiagram-v2
    [*] --> 未初期化
    未初期化 --> EEPROM読込: begin()
    EEPROM読込 --> RAM同期: バージョン一致
    EEPROM読込 --> デフォルト生成: 未初期化・不正
    デフォルト生成 --> RAM同期: EEPROMへ保存
    RAM同期 --> 変更あり: メニューでApply
    変更あり --> RAM同期: Save / commitSettings()成功
    変更あり --> RAM同期: uncommitSettings()
    変更あり --> 変更あり: commitSettings()失敗
```

設定はまず `SettingsManager` 内のRAMに保存されます。メニューの **Apply** はRAMの設定を更新し、**Save** が `commitSettings()` を呼び出してEEPROMへ永続化します。保存前の変更があるかは `dirty_` フラグで管理されます。

## メニュー操作の状態遷移

メニューの実体は `MenuState` に定義された `MenuConfig` と `MenuItem` です。`MenuManager` は「閲覧」と「値変更」の二つの操作モードを管理します。

```mermaid
stateDiagram-v2
    [*] --> Main: 起動
    Main --> Settings: ノブ押下
    Settings --> Submenu: SUBMENUを選択して押下
    Submenu --> Editing: VALUE_CHANGEを選択して押下
    Editing --> Editing: ノブ回転で値を変更
    Editing --> Submenu: 再度ノブ押下
    Submenu --> Submenu: APPLYを押下
    Submenu --> Submenu: SAVEを押下
    Submenu --> Settings: EXITを押下
    Settings --> Main: EXITを押下
```

| 操作 | 閲覧モード | 値変更モード |
| --- | --- | --- |
| ノブ回転 | カーソルを上下に移動 | 選択中パラメータを最小値〜最大値の範囲で変更 |
| ノブ押下 | 項目の種類に応じて遷移・実行 | 値変更モードを終了 |

項目種別は以下です。

| 種別 | 動作 |
| --- | --- |
| `SUBMENU` | 子メニューへ移動 |
| `VALUE_CHANGE` | パラメータ編集モードの開始／終了 |
| `APPLY` | 編集中のパラメータを `SettingsManager` へ反映 |
| `SAVE` | RAM上の設定をEEPROMへ保存 |
| `FUNCTION` | 現在は設定画面からの工場出荷時リセットに使用 |
| `EXIT` | 親メニューへ戻る |

## 関連する主なソース

| 役割 | ファイル |
| --- | --- |
| 起動と2コアループ | `src/main.cpp` |
| MIDI入力の制御 | `src/app/PedalsController/` |
| メニュー入力の制御 | `src/app/MenuController/` |
| メニュー状態・設定反映 | `src/app/MenuManager/` |
| 設定のRAM管理と永続化 | `src/app/SettingsManager/` |
| ハードウェア抽象化 | `lib/Hal/` |
| 実機ドライバ実装 | `lib/Pedal/`, `lib/ExpPedal/`, `lib/USBMIDI/`, `lib/DrawDisplay/`, `lib/EepromStrage/` |
