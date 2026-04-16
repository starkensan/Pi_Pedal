# Hardware

ハードウェア関連ファイルは、編集元と生成物を分けて配置します。

## Directory

- `kicad/`: KiCad の編集元プロジェクト、回路図、PCB、フットプリント
- `exports/pdf/`: 回路図や PCB などの PDF 出力
- `exports/gerber/`: 基板製造用のガーバー、ドリル、製造用パッケージ

## Notes

`exports/gerber/MIDI_Pedal/current/` は KiCad から出力したガーバーファイルです。

`exports/gerber/MIDI_Pedal/fabrication-package/` は、発注や共有に使う製造用パッケージを置く場所です。
