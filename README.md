# get_next_line_project

このディレクトリには、`get_next_line`関数の動作確認用のテストファイルが含まれています。

## 📁 ファイル構成

- `main.c` - 包括的なテストプログラム
- `Makefile_test` - テスト用Makefile
- `README_TEST.md` - このファイル

## 🚀 使用方法

### 1. 基本的なテスト

#### 通常版のテスト
```bash
# Makefileを使用
make -f Makefile_test test-regular

# または直接コンパイル
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o test_regular
./test_regular
```

#### ボーナス版のテスト
```bash
# Makefileを使用  
make -f Makefile_test test-bonus

# または直接コンパイル
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -D TEST_BONUS main.c get_next_line_bonus.c get_next_line_utils_bonus.c -o test_bonus
./test_bonus
```

### 2. 両方のバージョンをテスト
```bash
make -f Makefile_test test-all
```

### 3. 異なるBUFFER_SIZEでのテスト
```bash
make -f Makefile_test test-buffer-sizes
```

### 4. メモリリークチェック
```bash
# valgrindが必要
make -f Makefile_test test-leaks
```

### 5. クリーンアップ
```bash
make -f Makefile_test clean
```

## 🧪 テスト内容

### 基本テスト
- ✅ 空ファイル
- ✅ 1文字ファイル  
- ✅ 改行なし単一行
- ✅ 改行あり単一行
- ✅ 複数行ファイル
- ✅ 最終行に改行なし
- ✅ 改行のみのファイル
- ✅ 混合コンテンツ

### ボーナステスト（ボーナス版のみ）
- ✅ 複数ファイル同時読み込み
- ✅ ファイル間での交互読み込み
- ✅ 異なる長さのファイルの管理

### エラーハンドリングテスト
- ✅ 無効なファイルディスクリプタ
- ✅ 閉じられたファイルディスクリプタ

### パフォーマンステスト
- ✅ 大きなファイル（1000行）の読み込み
- ✅ 処理時間の測定

## 📊 テスト出力の見方

### 成功例
```
[✓] Test Name: PASS
```

### 失敗例  
```
[✗] Test Name: FAIL
```

### ファイル読み込み例
```
=== Test Description ===
Reading file: example.txt
Line 1: Hello World
Line 2: This is line 2
Total lines read: 2
```

## 🐛 トラブルシューティング

### コンパイルエラー
1. 必要なファイルが存在することを確認
2. ヘッダーファイルのパスが正しいことを確認
3. 関数の宣言と実装が一致していることを確認

### メモリリーク
```bash
# valgrindでメモリリークをチェック
valgrind --leak-check=full ./test_regular
```

### セグメンテーションフォルト
1. 配列の境界チェック
2. NULLポインタのチェック
3. メモリの適切な初期化

## 💡 カスタムテストの追加

`main.c`の`main`関数に新しいテストケースを追加できます：

```c
// カスタムテストファイル作成
create_test_file("custom.txt", "Custom content\nLine 2\n");

// テスト実行
test_single_file("custom.txt", "Custom Test");

// クリーンアップ
unlink("custom.txt");
```

## 📈 パフォーマンス最適化の確認

giant_line.txtのようなタイムアウトが発生していたテストケースでの改善を確認：

1. **改善前**: タイムアウト（数秒以上）
2. **改善後**: 高速処理（数ミリ秒）

処理時間とメモリ使用量の改善を実際に測定できます。

## 🎯 使用例

### 基本的な動作確認
```bash
# 最も基本的なテスト
make -f Makefile_test test-regular
```

### 徹底的なテスト
```bash
# すべてのテストを実行
make -f Makefile_test test-all
make -f Makefile_test test-buffer-sizes
make -f Makefile_test test-leaks
```

このテストスイートを使用することで、`get_next_line`関数の正しい動作と最適化の効果を確認できます！ 