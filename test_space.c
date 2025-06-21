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

void test_space_file(const char *filename, const char *description)
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
		
		// スペースと改行を可視化
		for (size_t i = 0; i < strlen(line); i++)
		{
			if (line[i] == ' ')
				printf("[SPACE]");
			else if (line[i] == '\n')
				printf("[NL]");
			else if (line[i] == '\t')
				printf("[TAB]");
			else
				printf("%c", line[i]);
		}
		printf("\"\n");
		free(line);
	}
	
	printf("Total lines read: %d\n", line_count);
	close(fd);
}

int main(void)
{
	printf("Space File Test\n");
	printf("===============\n");
	
	// 様々なスペースファイルのテストケース
	
	// 1. スペースのみ（改行なし）
	create_test_file("space_only.txt", "   ", 3);
	test_space_file("space_only.txt", "Space Only (No Newline)");
	
	// 2. スペース + 改行
	create_test_file("space_nl.txt", "   \n", 4);
	test_space_file("space_nl.txt", "Space + Newline");
	
	// 3. 複数行のスペース
	create_test_file("multi_space.txt", "  \n   \n    \n", 12);
	test_space_file("multi_space.txt", "Multiple Lines with Spaces");
	
	// 4. 空行とスペース行の混合
	create_test_file("mixed_space.txt", "\n  \n\n   \n", 9);
	test_space_file("mixed_space.txt", "Mixed Empty and Space Lines");
	
	// 5. スペースのみの長い行
	char long_spaces[1000];
	memset(long_spaces, ' ', 999);
	long_spaces[999] = '\0';
	create_test_file("long_space.txt", long_spaces, 999);
	test_space_file("long_space.txt", "Long Space Line (No Newline)");
	
	// 6. 長いスペース行 + 改行
	long_spaces[999] = '\n';
	create_test_file("long_space_nl.txt", long_spaces, 1000);
	test_space_file("long_space_nl.txt", "Long Space Line + Newline");
	
	// クリーンアップ
	unlink("space_only.txt");
	unlink("space_nl.txt");
	unlink("multi_space.txt");
	unlink("mixed_space.txt");
	unlink("long_space.txt");
	unlink("long_space_nl.txt");
	
	return (0);
} 