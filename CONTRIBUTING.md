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
