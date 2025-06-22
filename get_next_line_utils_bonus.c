/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skeita <skeita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:10:19 by skeita            #+#    #+#             */
/*   Updated: 2025/05/11 16:10:25 by skeita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
		if (result == READ_ERROR || expand_buffer(buffer, result) == -1)
			return (free(tmp), -1);
		if (result == FILE_TERMINATION)
			break ;
		i = 0;
		while (i < (size_t)result)
			i = (buffer->str[buffer->str_len + i] = tmp[i], i + 1);
		buffer->str_len += result;
		buffer->str[buffer->str_len] = '\0';
		if (exit_line_break(tmp, result))
			break ;
		free(tmp);
	}
	return (buffer->str_capacity = (free(tmp), buffer->str_len), 1);
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
