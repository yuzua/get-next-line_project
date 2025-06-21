#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "get_next_line.h"

// テスト用ファイル作成関数
void create_test_file(const char *filename, const char *content, size_t len)
{
	int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
	{
		write(fd, content, len);
		close(fd);
	}
}

void test_hello_space_file(const char *filename, const char *description)
{
	printf("\n=== %s ===\n", description);
	
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("Failed to open %s\n", filename);
		return;
	}
	
	char *line;
	int line_count = 0;
	
	printf("Reading file: %s\n", filename);
	while ((line = get_next_line(fd)) != NULL)
	{
		line_count++;
		printf("Line %d (len=%zu): \"", line_count, strlen(line));
		
		// 各文字を詳細に表示
		for (size_t i = 0; i < strlen(line); i++)
		{
			if (line[i] == ' ')
				printf("[SP]");
			else if (line[i] == '\n')
				printf("[NL]");
			else if (line[i] == '\t')
				printf("[TAB]");
			else if (line[i] == '\0')
				printf("[NULL]");
			else
				printf("%c", line[i]);
		}
		printf("\"\n");
		
		// 16進数表示も追加
		printf("Hex: ");
		for (size_t i = 0; i < strlen(line); i++)
		{
			printf("%02x ", (unsigned char)line[i]);
		}
		printf("\n");
		
		free(line);
	}
	
	printf("Total lines read: %d\n", line_count);
	close(fd);
}

int main(void)
{
	printf("HELLO Space Test\n");
	printf("================\n");
	
	// 様々な「HELLO <穴あき>」パターンのテスト
	
	// 1. "HELLO "（改行なし）
	create_test_file("hello_space.txt", "HELLO ", 6);
	test_hello_space_file("hello_space.txt", "HELLO + Space (No Newline)");
	
	// 2. "HELLO \n"（改行あり）
	create_test_file("hello_space_nl.txt", "HELLO \n", 7);
	test_hello_space_file("hello_space_nl.txt", "HELLO + Space + Newline");
	
	// 3. "HELLO   "（複数スペース、改行なし）
	create_test_file("hello_multi_space.txt", "HELLO   ", 8);
	test_hello_space_file("hello_multi_space.txt", "HELLO + Multiple Spaces (No Newline)");
	
	// 4. "HELLO   \n"（複数スペース、改行あり）
	create_test_file("hello_multi_space_nl.txt", "HELLO   \n", 9);
	test_hello_space_file("hello_multi_space_nl.txt", "HELLO + Multiple Spaces + Newline");
	
	// 5. 複数行での混合パターン
	create_test_file("hello_multi_lines.txt", "HELLO \nWORLD  \nTEST\n", 20);
	test_hello_space_file("hello_multi_lines.txt", "Multiple Lines with Trailing Spaces");
	
	// 6. タブ文字も含むパターン
	create_test_file("hello_tab.txt", "HELLO\t\n", 7);
	test_hello_space_file("hello_tab.txt", "HELLO + Tab + Newline");
	
	// 7. 混合スペース・タブパターン
	create_test_file("hello_mixed.txt", "HELLO \t \n", 9);
	test_hello_space_file("hello_mixed.txt", "HELLO + Space + Tab + Space + Newline");
	
	// 8. 長い文字列 + スペース
	create_test_file("long_hello_space.txt", "This is a long hello message with spaces at the end   \n", 56);
	test_hello_space_file("long_hello_space.txt", "Long Hello Message + Trailing Spaces");
	
	printf("\n=== Raw File Content Check ===\n");
	
	// ファイルの生データを確認
	int fd = open("hello_space.txt", O_RDONLY);
	if (fd != -1)
	{
		char buffer[100];
		int bytes_read = read(fd, buffer, sizeof(buffer));
		printf("Raw bytes in hello_space.txt (%d bytes):\n", bytes_read);
		for (int i = 0; i < bytes_read; i++)
		{
			printf("%02x ", (unsigned char)buffer[i]);
		}
		printf("\n");
		close(fd);
	}
	
	// クリーンアップ
	unlink("hello_space.txt");
	unlink("hello_space_nl.txt");
	unlink("hello_multi_space.txt");
	unlink("hello_multi_space_nl.txt");
	unlink("hello_multi_lines.txt");
	unlink("hello_tab.txt");
	unlink("hello_mixed.txt");
	unlink("long_hello_space.txt");
	
	return (0);
} 