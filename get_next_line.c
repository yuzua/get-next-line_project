/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skeita <skeita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:10:19 by skeita            #+#    #+#             */
/*   Updated: 2025/05/11 16:10:25 by skeita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_file_manager	*init_file_manager_buffer(void)
{
	t_file_manager	*buffer;
	size_t			i;

	buffer = (t_file_manager *)malloc(sizeof(t_file_manager) * 1);
	if (buffer == NULL)
		return (NULL);
	buffer->str = NULL;
	buffer->str_len = 0;
	buffer->str_capacity = 0;
	buffer->read_index = 0;
	buffer->not_read_len = 0;
	i = 0;
	while (i < BUFFER_SIZE)
	{
		buffer->not_read_str[i] = '\0';
		i++;
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	char					*line;
	static t_file_manager	*buffer;
	int						read_result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	
	// ①bufferの初期化
	if (buffer == NULL)
	{
		buffer = init_file_manager_buffer();
		if (buffer == NULL)
			return (NULL);
	}
	
	// ②buffer->not_read_strに改行が含まれている場合の処理
	line = extract_line_from_not_read(buffer);
	if (line != NULL)
		return (line);
	// ③ファイルから改行が含まれるまでbuffer->strへ値の読み取り
	read_result = read_file(fd, buffer);
	if (read_result == -1)
		return (free(buffer->str), free(buffer), buffer = NULL, NULL);
	
	// ④buffer->not_read_strとbuffer->strを結合し改行まで値を読み取り
	line = combine_and_extract_line(buffer);
	free(buffer->str);
	buffer->str = NULL;
	buffer->str_len = 0;
	buffer->str_capacity = 0;
	buffer->read_index = 0;
	if (line == NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	return (line);
}
