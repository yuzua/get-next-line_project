/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skeita <skeita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:10:19 by skeita            #+#    #+#             */
/*   Updated: 2025/05/11 16:10:25 by skeita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	char					*line;
	static t_file_manager	*buffer_list;
	t_file_manager			*buffer;
	int						read_result;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = find_or_create_buffer(&buffer_list, fd);
	if (buffer == NULL)
		return (NULL);
	read_result = read_file(fd, buffer);
	if (read_result == -1)
	{
		remove_buffer_list(&buffer_list, fd);
		return (NULL);
	}
	line = read_line(buffer);
	if (line == NULL)
		remove_buffer_list(&buffer_list, fd);
	return (line);
}

t_file_manager	*find_or_create_buffer(t_file_manager **buffer_list, int fd)
{
	t_file_manager	*current;

	current = *buffer_list;
	while (current != NULL)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	current = init_file_manager_buffer(fd);
	if (current == NULL)
		return (NULL);
	current->next = *buffer_list;
	*buffer_list = current;
	return (current);
}

t_file_manager	*init_file_manager_buffer(int fd)
{
	t_file_manager	*buffer;

	buffer = (t_file_manager *)malloc(sizeof(t_file_manager) * 1);
	if (buffer == NULL)
		return (NULL);
	buffer->fd = fd;
	buffer->str = NULL;
	buffer->str_len = 0;
	buffer->str_capacity = 0;
	buffer->read_index = 0;
	buffer->next = NULL;
	return (buffer);
}

void	remove_buffer_list(t_file_manager **buffer_list, int fd)
{
	t_file_manager	*current;
	t_file_manager	*previous;

	current = *buffer_list;
	previous = NULL;
	while (current != NULL)
	{
		if (current->fd == fd)
		{
			if (previous == NULL)
				*buffer_list = current->next;
			else
				previous->next = current->next;
			if (current->str != NULL)
				free(current->str);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
