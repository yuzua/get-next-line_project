/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skeita <skeita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:10:19 by skeita            #+#    #+#             */
/*   Updated: 2025/05/11 16:10:25 by skeita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	expand_buffer(t_file_manager *buffer, size_t additional_size)
{
	char	*str;
	size_t	capacity;
	size_t	i;

	if (buffer->str_len + additional_size <= buffer->str_capacity)
		return (1);
	capacity = 16;
	buffer->str_capacity != 0 && (capacity = buffer->str_capacity, 0);
	while (capacity < buffer->str_len + additional_size)
		capacity *= 2;
	str = (char *)malloc(sizeof(char) * (capacity + 1));
	if (str == NULL)
		return (-1);
	i = 0;
	if (buffer->str != NULL && buffer->str_len > 0)
	{
		while (i < buffer->str_len)
		{
			str[i] = buffer->str[i];
			i++;
		}
		free(buffer->str);
	}
	buffer->str_capacity = (buffer->str = str, capacity);
	return (1);
}

int	read_file(int fd, t_file_manager *buffer)
{
	char	*tmp;
	int		result;
	size_t	i;

	while (true)
	{
		tmp = read_str_calloc(BUFFER_SIZE);
		if (tmp == NULL)
			return (-1);
		result = read(fd, tmp, BUFFER_SIZE);
		if (result == READ_ERROR)
			return (free(tmp), -1);
		if (result == FILE_TERMINATION)
			break ;
		
		// バッファを拡張
		if (expand_buffer(buffer, result) == -1)
			return (free(tmp), -1);
		
		i = 0;
		while (i < (size_t)result)
			i = (buffer->str[buffer->str_len + i] = tmp[i], i + 1);
		buffer->str_len += result;
		buffer->str[buffer->str_len] = '\0';
		if (exit_line_break(tmp, result))
			break ;
		free(tmp);
	}
	free(tmp);
	return (1);
}

char	*read_str_calloc(size_t str_len)
{
	char	*ptr;
	size_t	ptr_i;

	ptr = (char *)malloc(sizeof(char) * str_len);
	if (ptr == NULL)
		return (NULL);
	ptr_i = 0;
	while (ptr_i < str_len)
		ptr[ptr_i++] = '\0';
	return (ptr);
}

bool	exit_line_break(char *str, size_t str_length)
{
	size_t	str_i;

	if (str == NULL)
		return (false);
	str_i = 0;
	while (str_i < str_length)
		if (str[str_i++] == '\n')
			return (true);
	return (false);
}

char	*read_line(t_file_manager *buffer)
{
	char	*line;
	size_t	line_i;
	size_t	line_len;

	if (buffer->str == NULL)
		return (NULL);
	line_len = 0;
	while (buffer->read_index + line_len < buffer->str_len
		&& buffer->str[buffer->read_index + line_len] != '\n'
		&& buffer->str[buffer->read_index + line_len] != '\0')
		line_len++;
	if (buffer->str[buffer->read_index + line_len] == '\n')
		line_len++;
	if (line_len == 0)
		return (NULL);
	line = read_str_calloc(line_len + 1);
	if (line == NULL)
		return (NULL);
	line_i = 0;
	while (line_i < line_len)
	{
		line[line_i] = buffer->str[buffer->read_index + line_i];
		line_i++;
	}
	return (line[line_i] = '\0', buffer->read_index += line_len, line);
}

char	*extract_line_from_not_read(t_file_manager *buffer)
{
	char	*line;
	size_t	line_len;
	size_t	line_i;
	size_t	i;
	size_t	remaining;

	i = 0;
	while (i < buffer->not_read_len)
	{
		if (buffer->not_read_str[i] == '\n')
		{
			line_len = i + 1;
			line = read_str_calloc(line_len + 1);
			if (line == NULL)
				return (NULL);
			line_i = 0;
			while (line_i < line_len)
			{
				line[line_i] = buffer->not_read_str[line_i];
				line_i++;
			}
			line[line_i] = '\0';
			remaining = (line_i = 0, buffer->not_read_len - line_len);
			while (line_i < remaining)
			{
				buffer->not_read_str[line_i] = buffer->not_read_str[line_len + line_i];
				line_i++;
			}
			return (buffer->not_read_len = remaining, line);
		}
		i++;
	}
	return (NULL);
}

char	*combine_and_extract_line(t_file_manager *buffer)
{
	char	*combined_data;
	char	*line;
	size_t	total_len;
	size_t	line_len;
	size_t	i;
	size_t	remaining;

	total_len = buffer->not_read_len + buffer->str_len;
	if (total_len == 0)
		return (NULL);
	combined_data = read_str_calloc(total_len + 1);
	if (combined_data == NULL)
		return (NULL);
	i = 0;
	while (i < buffer->not_read_len)
		i = (combined_data[i] = buffer->not_read_str[i], i + 1);
	i = 0;
	while (i < buffer->str_len)
		i = (combined_data[buffer->not_read_len + i] = buffer->str[i], i + 1);
	line_len = (combined_data[total_len] = '\0', 0);
	while (line_len < total_len && combined_data[line_len] != '\n')
		line_len++;
	if (line_len < total_len && combined_data[line_len] == '\n')
		line_len++;
	if (line_len == 0)
		return (free(combined_data), NULL);
	line = read_str_calloc(line_len + 1);
	if (line == NULL)
		return (free(combined_data), NULL);
	i = 0;
	while (i < line_len)
		i = (line[i] = combined_data[i], i + 1);
	line[i] = '\0';
	remaining = total_len - line_len;
	if (remaining > 0 && remaining < BUFFER_SIZE)
	{
		i = 0;
		while (i < remaining)
			i = (buffer->not_read_str[i] = combined_data[line_len + i], i + 1);
		buffer->not_read_len = remaining;
	}
	else
		buffer->not_read_len = 0;
	return (free(combined_data), line);
}
