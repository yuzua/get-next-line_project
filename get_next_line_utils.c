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

int	read_file(int fd, t_file_manager *buffer, size_t i)
{
	char	*tmp;
	int		result;

	while (true)
	{
		tmp = ((i = 0), (char *)malloc(sizeof(char) * BUFFER_SIZE));
		if (tmp == NULL)
			return (-1);
		result = ((i = 0), read(fd, tmp, BUFFER_SIZE));
		if (result == READ_ERROR)
			return (free(tmp), -1);
		if (result == FILE_TERMINATION)
			break ;
		if (expand_buffer(buffer, result) == -1)
			return (free(tmp), -1);
		while (i < (size_t)result)
			i = (buffer->str[buffer->str_len + i] = tmp[i], i + 1);
		buffer->str_len += result;
		buffer->str[buffer->str_len] = '\0';
		if (exit_line_break(tmp, result))
			break ;
		free(tmp);
	}
	return (free(tmp), 1);
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

void	update_remaining_data(t_file_manager *buffer, size_t str_line_len)
{
	size_t	i;

	buffer->not_read_len = buffer->str_len - str_line_len;
	if (buffer->not_read_len > 0 && buffer->not_read_len < BUFFER_SIZE)
	{
		i = 0;
		while (i < buffer->not_read_len)
		{
			buffer->not_read_str[i] = buffer->str[str_line_len + i];
			i++;
		}
	}
	else
		buffer->not_read_len = 0;
}

char	*extract_line_from_not_read(t_file_manager *buffer, size_t i)
{
	char	*line;
	size_t	l_i;
	size_t	remaining;

	while (i < buffer->not_read_len)
	{
		if (buffer->not_read_str[i++] != '\n')
			continue ;
		line = (i--, (char *)malloc(sizeof(char) * ((i + 1) + 1)));
		if (line == NULL)
			return (NULL);
		l_i = 0;
		while (l_i < (i + 1))
			l_i = ((line[l_i]
						= buffer->not_read_str[l_i]), l_i + 1);
		line[l_i] = '\0';
		remaining = ((l_i = 0), buffer->not_read_len - (i + 1));
		while (l_i < remaining)
			l_i = ((buffer->not_read_str[l_i]
						= buffer->not_read_str[(i + 1) + l_i]), l_i + 1);
		return (buffer->not_read_len = remaining, line);
	}
	return (NULL);
}

char	*combine_and_extract_line(t_file_manager *buffer, size_t i)
{
	char	*line;
	size_t	line_len;
	size_t	str_line_len;

	if (buffer->not_read_len + buffer->str_len == 0)
		return (NULL);
	str_line_len = 0;
	while (str_line_len < buffer->str_len && buffer->str[str_line_len] != '\n')
		str_line_len++;
	if (str_line_len < buffer->str_len && buffer->str[str_line_len] == '\n')
		str_line_len++;
	line_len = buffer->not_read_len + str_line_len;
	if (line_len == 0)
		return (NULL);
	line = (char *)malloc(sizeof(char) * (line_len + 1));
	if (line == NULL)
		return (NULL);
	while (i < buffer->not_read_len)
		i = ((line[i] = buffer->not_read_str[i]), i + 1);
	i = 0;
	while (i < str_line_len)
		i = (line[buffer->not_read_len + i] = buffer->str[i], i + 1);
	line[line_len] = '\0';
	return (update_remaining_data(buffer, str_line_len), line);
}
