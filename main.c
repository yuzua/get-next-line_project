#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// 通常版をテストする場合は以下をコメントアウト
// #define TEST_BONUS

#ifdef TEST_BONUS
#include "get_next_line_bonus.h"
#else
#include "get_next_line.h"
#endif

// カラー定義
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

// テスト用ファイル作成関数
void create_test_file(const char *filename, const char *content)
{
	int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
	{
		write(fd, content, strlen(content));
		close(fd);
	}
}

// テスト結果表示
void print_test_result(const char *test_name, int passed)
{
	printf("[%s] %s: %s%s%s\n", 
		passed ? "✓" : "✗",
		test_name,
		passed ? GREEN : RED,
		passed ? "PASS" : "FAIL",
		RESET);
}

// 単一ファイルテスト
void test_single_file(const char *filename, const char *description)
{
	printf("\n" CYAN "=== %s ===" RESET "\n", description);
	
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf(RED "Failed to open %s" RESET "\n", filename);
		return;
	}
	
	char *line;
	int line_count = 0;
	
	printf(YELLOW "Reading file: %s" RESET "\n", filename);
	while ((line = get_next_line(fd)) != NULL)
	{
		line_count++;
		printf("Line %d: %s", line_count, line);
		if (line[strlen(line) - 1] != '\n')
			printf("\n");
		free(line);
	}
	
	printf("Total lines read: %d\n", line_count);
	close(fd);
}

#ifdef TEST_BONUS
// 複数ファイル同時テスト（ボーナス版のみ）
void test_multiple_files()
{
	printf("\n" MAGENTA "=== BONUS: Multiple Files Test ===" RESET "\n");
	
	// テストファイル作成
	create_test_file("test1.txt", "File1 Line1\nFile1 Line2\nFile1 Line3\n");
	create_test_file("test2.txt", "File2 Line1\nFile2 Line2\n");
	create_test_file("test3.txt", "File3 Line1\nFile3 Line2\nFile3 Line3\nFile3 Line4\n");
	
	int fd1 = open("test1.txt", O_RDONLY);
	int fd2 = open("test2.txt", O_RDONLY);
	int fd3 = open("test3.txt", O_RDONLY);
	
	if (fd1 == -1 || fd2 == -1 || fd3 == -1)
	{
		printf(RED "Failed to open test files" RESET "\n");
		return;
	}
	
	char *line;
	printf(YELLOW "Interleaved reading from 3 files:" RESET "\n");
	
	// 交互に読み込み
	for (int round = 1; round <= 5; round++)
	{
		printf("\n--- Round %d ---\n", round);
		
		// File 1
		line = get_next_line(fd1);
		if (line)
		{
			printf("FD%d: %s", fd1, line);
			free(line);
		}
		else
			printf("FD%d: EOF\n", fd1);
		
		// File 2
		line = get_next_line(fd2);
		if (line)
		{
			printf("FD%d: %s", fd2, line);
			free(line);
		}
		else
			printf("FD%d: EOF\n", fd2);
		
		// File 3
		line = get_next_line(fd3);
		if (line)
		{
			printf("FD%d: %s", fd3, line);
			free(line);
		}
		else
			printf("FD%d: EOF\n", fd3);
	}
	
	close(fd1);
	close(fd2);  
	close(fd3);
	
	// テストファイル削除
	unlink("test1.txt");
	unlink("test2.txt");
	unlink("test3.txt");
}
#endif

// エラーハンドリングテスト
void test_error_handling()
{
	printf("\n" BLUE "=== Error Handling Tests ===" RESET "\n");
	
	// 無効なファイルディスクリプタ
	char *line = get_next_line(-1);
	print_test_result("Invalid FD (-1)", line == NULL);
	if (line) free(line);
	
	line = get_next_line(999);
	print_test_result("Invalid FD (999)", line == NULL);
	if (line) free(line);
	
	// 閉じられたファイルディスクリプタ
	int fd = open("test.txt", O_RDONLY);
	if (fd != -1)
	{
		close(fd);
		line = get_next_line(fd);
		print_test_result("Closed FD", line == NULL);
		if (line) free(line);
	}
}

// パフォーマンステスト用の大きなファイル
void test_large_file()
{
	printf("\n" CYAN "=== Large File Test ===" RESET "\n");
	
	// 大きなファイルを作成
	printf("Creating large test file...\n");
	int fd = open("large_test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
	{
		// 1000行の長い行を作成
		for (int i = 0; i < 1000; i++)
		{
			char buffer[1000];
			snprintf(buffer, sizeof(buffer), 
				"This is line %d with some long content to test buffer management - %s\n", 
				i + 1, "Lorem ipsum dolor sit amet consectetur adipiscing elit");
			write(fd, buffer, strlen(buffer));
		}
		close(fd);
	}
	
	// 読み込みテスト
	printf("Reading large file...\n");
	fd = open("large_test.txt", O_RDONLY);
	if (fd != -1)
	{
		char *line;
		int count = 0;
		clock_t start = clock();
		
		while ((line = get_next_line(fd)) != NULL)
		{
			count++;
			free(line);
		}
		
		clock_t end = clock();
		double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
		
		printf("Read %d lines in %.4f seconds\n", count, time_taken);
		print_test_result("Large file test", count == 1000);
		
		close(fd);
	}
	
	unlink("large_test.txt");
}

int main(void)
{
	printf(GREEN "=========================================\n");
#ifdef TEST_BONUS
	printf("    GET_NEXT_LINE BONUS VERSION TEST\n");
#else
	printf("    GET_NEXT_LINE REGULAR VERSION TEST\n");
#endif
	printf("=========================================" RESET "\n");
	
	// テストファイル作成
	create_test_file("empty.txt", "");
	create_test_file("single_char.txt", "a");
	create_test_file("single_line.txt", "Hello World");
	create_test_file("single_line_nl.txt", "Hello World\n");
	create_test_file("multiple_lines.txt", "Line 1\nLine 2\nLine 3\n");
	create_test_file("no_final_nl.txt", "Line 1\nLine 2\nLine 3");
	create_test_file("only_nl.txt", "\n\n\n");
	create_test_file("mixed.txt", "Short\nThis is a longer line with more content\n\nEmpty line above\n");
	
	// HELLO <穴あき>テスト用
	create_test_file("hello_space.txt", "HELLO ");
	create_test_file("hello_space_nl.txt", "HELLO \n");
	create_test_file("hello_multi_space.txt", "HELLO   ");
	create_test_file("hello_tab.txt", "HELLO\t");
	create_test_file("hello_mixed.txt", "HELLO \t ");
	
	// 基本テスト
	test_single_file("empty.txt", "Empty File Test");
	test_single_file("single_char.txt", "Single Character Test");
	test_single_file("single_line.txt", "Single Line (No NL) Test");
	test_single_file("single_line_nl.txt", "Single Line (With NL) Test");
	test_single_file("multiple_lines.txt", "Multiple Lines Test");
	test_single_file("no_final_nl.txt", "No Final Newline Test");
	test_single_file("only_nl.txt", "Only Newlines Test");
	test_single_file("mixed.txt", "Mixed Content Test");
	
	// HELLO <穴あき>テスト
	test_single_file("hello_space.txt", "HELLO + Space (No NL) Test");
	test_single_file("hello_space_nl.txt", "HELLO + Space + NL Test");
	test_single_file("hello_multi_space.txt", "HELLO + Multi Space Test");
	test_single_file("hello_tab.txt", "HELLO + Tab Test");
	test_single_file("hello_mixed.txt", "HELLO + Mixed Whitespace Test");
	
#ifdef TEST_BONUS
	// ボーナステスト
	test_multiple_files();
#endif
	
	// エラーハンドリングテスト
	test_error_handling();
	
	// パフォーマンステスト
	test_large_file();
	
	// テストファイル削除
	unlink("empty.txt");
	unlink("single_char.txt");
	unlink("single_line.txt");
	unlink("single_line_nl.txt");  
	unlink("multiple_lines.txt");
	unlink("no_final_nl.txt");
	unlink("only_nl.txt");
	unlink("mixed.txt");
	unlink("hello_space.txt");
	unlink("hello_space_nl.txt");
	unlink("hello_multi_space.txt");
	unlink("hello_tab.txt");
	unlink("hello_mixed.txt");
	
	printf("\n" GREEN "=========================================" RESET "\n");
	printf(GREEN "           TEST COMPLETED" RESET "\n");
	printf(GREEN "=========================================" RESET "\n");
	
	return (0);
} 