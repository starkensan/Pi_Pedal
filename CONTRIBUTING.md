# Contributing

## Branch Naming

Issue に対応する作業ブランチは、以下の形式にしてください。

```text
feature-{issue番号}
```

例:

```text
feature-38
```

Issue がない作業は、先に Issue を作成してからブランチを作成してください。

## Commit Message

コミットメッセージは、以下の形式にしてください。

```text
{type}: {コミット内容}
```

主な `type`:

- `feat`: 機能追加、仕様追加
- `fix`: 不具合修正
- `docs`: ドキュメント変更
- `refactor`: 振る舞いを変えないコード整理
- `test`: テスト追加、修正
- `chore`: ビルド設定や補助作業の変更

例:

```text
feat: MIDI チャンネル設定を追加
docs: ブランチ命名ルールを追加
```

## Pull Request Title

Pull Request のタイトルは、以下の形式にしてください。

```text
{type}: {PR内容} #{issue番号}
```

`type` はコミットメッセージと同じものを使用してください。

例:

```text
docs: 開発ルールを追加 #38
feat: MIDI チャンネル設定を追加 #42
```

## Pull Request Checks

Pull Request を作成したら、GitHub Actions の `PlatformIO Build` が成功していることを確認してください。

- 対象 workflow: `.github/workflows/platformio-build.yml`
- 実行タイミング: `pull_request`、および `develop` への `push`
- ビルド対象: `pico`, `pico-debug`

ローカルで事前確認する場合は、以下を実行してください。

```bash
pio run -e pico
pio run -e pico-debug
```

PR では、ビルド成功を確認内容に記載してください。

## Source Layout

モジュール配置は以下の方針で統一します。

- `src/app/`: アプリケーション固有の制御、画面、状態管理
- `lib/`: 再利用しやすいドライバ、ハードウェア抽象、共通部品
- `include/`: `src/`、`lib/`、`test/` から参照する共通公開ヘッダ

新しいアプリケーション機能は `src/app/` 配下へ追加してください。
