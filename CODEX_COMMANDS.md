# Codex コマンドまとめ

このドキュメントは、`codex-cli 0.125.0` の `--help` 出力をもとに、よく使うコマンドを日本語で整理したものです。

## 基本構文

```bash
codex [OPTIONS] [PROMPT]
codex [OPTIONS] <COMMAND> [ARGS]
```

- サブコマンドを付けない場合は、対話モードで起動します。
- `PROMPT` を渡すと、その内容を最初の指示として開始できます。

例:

```bash
codex
codex "このリポジトリの構成を説明して"
codex -C /path/to/project
```

## よく使う主要コマンド

### 1. 対話モード

```bash
codex
```

- ターミナル上で対話的に Codex を使います。
- サブコマンドなしの標準的な使い方です。

よく併用するオプション:

- `-C, --cd <DIR>`: 作業ディレクトリを指定
- `-m, --model <MODEL>`: 使用モデルを指定
- `-s, --sandbox <MODE>`: サンドボックスモードを指定
- `-a, --ask-for-approval <POLICY>`: コマンド実行時の承認ポリシーを指定
- `--search`: Web 検索を有効化
- `--full-auto`: 低摩擦な自動実行モード

例:

```bash
codex -C . --search
codex -C . -s workspace-write -a on-request
```

### 2. `codex exec`

```bash
codex exec [PROMPT]
```

- 非対話モードで Codex を実行します。
- バッチ処理やスクリプトからの利用に向いています。

例:

```bash
codex exec "READMEを要約して"
echo "変更点をレビューして" | codex exec -
codex exec -o result.txt "このコードの改善点を3つ挙げて"
```

便利なオプション:

- `--json`: JSONL 形式でイベントを出力
- `-o, --output-last-message <FILE>`: 最終メッセージをファイル保存
- `--ephemeral`: セッションをディスクへ保存しない
- `--skip-git-repo-check`: Git 管理外でも実行
- `--ignore-user-config`: ユーザー設定を無視して実行

### 3. `codex review`

```bash
codex review [PROMPT]
```

- コードレビューを非対話で実行します。
- 差分やコミット単位での確認に向いています。

例:

```bash
codex review
codex review --uncommitted
codex review --base main
codex review --commit HEAD~1
codex review "パフォーマンスと安全性を重点的に見て"
```

便利なオプション:

- `--uncommitted`: 未コミット変更をレビュー
- `--base <BRANCH>`: 指定ブランチとの差分をレビュー
- `--commit <SHA>`: 特定コミットの変更をレビュー
- `--title <TITLE>`: レビュー表示用タイトルを指定

### 4. `codex resume`

```bash
codex resume [SESSION_ID]
```

- 以前の対話セッションを再開します。

例:

```bash
codex resume --last
codex resume 123e4567-e89b-12d3-a456-426614174000
codex resume --all
```

便利なオプション:

- `--last`: 直近のセッションを再開
- `--all`: カレントディレクトリ以外のセッションも表示
- `--include-non-interactive`: 非対話セッションも候補に含める

### 5. `codex fork`

```bash
codex fork [SESSION_ID]
```

- 既存セッションを分岐して新しいセッションを作ります。
- 元の会話を残しつつ、別案を試したいときに便利です。

例:

```bash
codex fork --last
codex fork 123e4567-e89b-12d3-a456-426614174000
```

### 6. `codex apply`

```bash
codex apply <TASK_ID>
```

- Codex エージェントが生成した最新 diff を、ローカルのワーキングツリーに `git apply` 形式で反映します。
- Cloud や外部タスク連携後の反映用途で使います。

### 7. `codex login` / `codex logout`

```bash
codex login
codex login status
codex logout
```

- 認証状態の管理を行います。

例:

```bash
codex login status
printenv OPENAI_API_KEY | codex login --with-api-key
codex logout
```

### 8. `codex mcp`

```bash
codex mcp <COMMAND>
```

- 外部 MCP サーバーを管理します。

主なサブコマンド:

- `list`: 一覧表示
- `get`: 設定確認
- `add`: 追加
- `remove`: 削除
- `login`: 認証
- `logout`: 認証解除

例:

```bash
codex mcp list
codex mcp get <name>
```

### 9. `codex plugin`

```bash
codex plugin <COMMAND>
```

- Codex プラグインを管理します。
- 現時点の `--help` では `marketplace` サブコマンドが確認できます。

例:

```bash
codex plugin marketplace --help
```

### 10. `codex completion`

```bash
codex completion [SHELL]
```

- シェル補完スクリプトを生成します。

例:

```bash
codex completion bash
codex completion zsh
codex completion fish
```

### 11. `codex sandbox`

```bash
codex sandbox <COMMAND>
```

- Codex 提供のサンドボックス内でコマンドを実行します。

主なサブコマンド:

- `linux`
- `macos`
- `windows`

例:

```bash
codex sandbox linux --help
```

### 12. `codex features`

```bash
codex features <COMMAND>
```

- フィーチャーフラグの確認や有効化を行います。

主なサブコマンド:

- `list`
- `enable`
- `disable`

例:

```bash
codex features list
```

## そのほかのコマンド

トップレベル `--help` で確認できたコマンド:

- `mcp-server`: Codex を MCP サーバーとして起動
- `app-server`: アプリサーバー関連機能
- `debug`: デバッグ用機能
- `cloud`: Codex Cloud のタスク参照とローカル反映
- `exec-server`: スタンドアロン実行サーバー

必要に応じて、以下で個別ヘルプを確認できます。

```bash
codex <command> --help
```

## よく使うグローバルオプション

多くのコマンドで共通して使えるものです。

- `-c, --config <key=value>`: 設定値の上書き
- `--enable <FEATURE>`: 機能を有効化
- `--disable <FEATURE>`: 機能を無効化
- `-m, --model <MODEL>`: モデル指定
- `-p, --profile <CONFIG_PROFILE>`: プロファイル指定
- `-i, --image <FILE>`: 初期入力に画像を添付
- `-h, --help`: ヘルプ表示
- `-V, --version`: バージョン表示

例:

```bash
codex -V
codex -m gpt-5 -C .
codex -c model="gpt-5" -c shell_environment_policy.inherit=all
```

## サンドボックスと承認ポリシー

### `--sandbox`

実行モード:

- `read-only`
- `workspace-write`
- `danger-full-access`

例:

```bash
codex -s read-only
codex -s workspace-write
```

### `--ask-for-approval`

承認ポリシー:

- `untrusted`
- `on-failure`  
  非推奨。ヘルプ上でも `DEPRECATED` とされています。
- `on-request`
- `never`

例:

```bash
codex -a on-request
codex exec -a never "このディレクトリを解析して"
```

## まず使うならこの3つ

### 対話で使う

```bash
codex
```

### 非対話で1回だけ実行する

```bash
codex exec "やってほしいこと"
```

### 差分レビューをする

```bash
codex review --uncommitted
```

## 補足

- コマンド体系はバージョンによって変わる可能性があります。
- 詳細は常に `codex --help` と `codex <subcommand> --help` を優先してください。
