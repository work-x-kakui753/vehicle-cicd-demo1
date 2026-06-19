# Demo Scenario

このデモは、機密情報を含まないC言語サンプルで車載ソフト風CI/CDの流れを確認するためのものです。

## 説明の流れ

1. 開発者がGitHubへpushする
2. GitHub Actionsが自動起動する
3. Windows Runnerで `scripts\ci_windows.bat` を実行する
4. Cコードをビルドする
5. 温度計測、照明オンオフ、CAN通信判定の車載風テストを実行する
6. EXE、ログ、Markdownレポートを成果物として保存する

## 次段階の展開

次段階では、GitHub-hosted runnerの代わりに、CS+導入済みのWindows self-hosted runnerへ置き換えることで、本番向けツールチェーンに近づけます。
