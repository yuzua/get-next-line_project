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

	buffer = (t_file_manager *)malloc(sizeof(t_file_manager) * 1);
	if (buffer == NULL)
		return (NULL);
	buffer->str = NULL;
	buffer->str_len = 0;
	buffer->str_capacity = 0;
	buffer->read_index = 0;
	return (buffer);
}

char	*get_next_line(int fd)
{
	char					*line;
	static t_file_manager	*buffer;
	int						read_result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (buffer == NULL)
	{
		buffer = init_file_manager_buffer();
		if (buffer == NULL)
			return (NULL);
	}
	read_result = read_file(fd, buffer);
	if (read_result == -1)
		return (free(buffer->str), free(buffer), buffer = NULL, NULL);
	line = read_line(buffer);
	if (line == NULL)
		(free(buffer->str), free(buffer), buffer = NULL);
	return (line);
}
