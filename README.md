# vehicle-cicd-demo1

Windows GitHub Actions上で、C言語のビルド・テスト・成果物保存を見せるための車載ソフト風CI/CDデモです。機密情報や実案件名を含まず、GitHub-hosted runnerだけで動く構成にしています。

## このデモの目的

- `git push` を契機にGitHub Actionsが自動起動する流れを見せる
- Windows Runner上でバッチ処理が実行されることを見せる
- MSVCでC言語コードをビルドし、テストを実行する流れを見せる
- ログ、実行ファイル、Markdownレポートをartifactとして残す

## お客様に見せるポイント

- `.github/workflows/windows-c-ci.yml` が `push`、`pull_request`、`workflow_dispatch` で起動する
- `scripts\ci_windows.bat` がCIの中核として動作する
- `src/` のCコードをビルドし、`tests/` の車載ソフト風テストを実行する
- Actions画面のステップ名だけで処理内容が追える
- `build/` 配下にEXE、ログ、レポートがまとまる

## CIで実施していること

GitHub ActionsはWindows runner上で以下を実行します。

1. ソースコードをcheckout
2. MSVCのコマンドライン環境を準備
3. `scripts\ci_windows.bat prepare` でビルド用ディレクトリを作成
4. `scripts\ci_windows.bat build` でCコードをビルド
5. `scripts\ci_windows.bat test` でテスト実行
6. `scripts\ci_windows.bat demo` でデモ用EXEを実行
7. `scripts\ci_windows.bat summary` でMarkdown形式のCIサマリを生成
8. `build/` 配下をartifactとしてアップロード

## テスト内容

### 温度計測

- `check_temperature_status(int temperature_c)` で温度状態を判定
- `25` は正常
- `85` は警告
- `105` は異常
- `-40` 未満、`150` 超過はセンサー異常

### 照明オンオフ

- `decide_headlamp_state(int ambient_lux, int ignition_on)` でヘッドランプ状態を判定
- 暗いかつイグニッションONでライトON
- 明るい、またはイグニッションOFFでライトOFF

### CAN通信判定

- `validate_can_frame(unsigned int can_id, int dlc)` でCANフレームの妥当性を判定
- 想定ID `0x123` と `0x456` を許可
- DLCは `0` から `8` の範囲を許可
- 想定外IDやDLC超過は異常

## 成果物の見方

CI完了後、artifactとして次が残ります。

```text
build/
├─ vehicle_demo.exe
├─ vehicle_tests.exe
├─ logs/
│  ├─ build.log
│  ├─ test.log
│  └─ run_demo.log
└─ reports/
   └─ ci_summary.md
```

- `vehicle_demo.exe`: デモ用アプリ本体
- `vehicle_tests.exe`: テスト実行バイナリ
- `build.log`: MSVCビルドログ
- `test.log`: テスト結果ログ
- `run_demo.log`: デモ実行ログ
- `ci_summary.md`: CIの要点をMarkdownでまとめたレポート

## ローカルWindowsでの実行方法

Visual Studio 2022 Build Tools、またはVisual Studio 2022のC++開発環境を入れたWindows端末で実行します。`cl.exe` が使えるDeveloper Command Promptを開き、リポジトリ直下で以下を実行します。

```bat
scripts\ci_windows.bat
```

フェーズ単位で確認したい場合は以下も使えます。

```bat
scripts\ci_windows.bat prepare
scripts\ci_windows.bat build
scripts\ci_windows.bat test
scripts\ci_windows.bat demo
scripts\ci_windows.bat summary
```

## 本番ツールへ置き換える場合の考え方

- `scripts\ci_windows.bat` のビルド部分をCS+や専用コンパイラ呼び出しへ差し替える
- テスト実行部をWinAMSや社内試験ツールへ差し替える
- GitHub-hosted runnerを、社内ライセンスや専用ツールが入ったWindows self-hosted runnerへ置き換える
- 現在の `build.log`、`test.log`、`ci_summary.md` の生成位置を維持すれば、artifact運用はほぼそのまま流用できる

## 関連ファイル

- [workflow](.github/workflows/windows-c-ci.yml)
- [batch script](scripts/ci_windows.bat)
- [demo scenario](docs/demo_scenario.md)
- [CI flow](docs/ci_flow.md)
