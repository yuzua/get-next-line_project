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

t_file_manager	*init_file_manager_buffer(int fd)
{
	t_file_manager	*buffer;
	size_t			i;

	buffer = (t_file_manager *)malloc(sizeof(t_file_manager) * 1);
	if (buffer == NULL)
		return (NULL);
	buffer->fd = fd;
	buffer->str = NULL;
	buffer->str_len = 0;
	buffer->str_capacity = 0;
	buffer->read_index = 0;
	buffer->not_read_len = 0;
	buffer->next = NULL;
	i = 0;
	while (i < BUFFER_SIZE)
	{
		buffer->not_read_str[i] = '\0';
		i++;
	}
	return (buffer);
}

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

char	*get_next_line(int fd)
{
	char					*line;
	static t_file_manager	*buffer_list;
	t_file_manager			*buffer;
	int						read_result;
	size_t					i;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = find_or_create_buffer(&buffer_list, fd);
	if (buffer == NULL)
		return (NULL);
	i = 0;
	line = extract_line_from_not_read(buffer, i);
	if (line != NULL)
		return (line);
	read_result = ((i = 0), read_file(fd, buffer, i));
	if (read_result == -1)
		return (remove_buffer_list(&buffer_list, fd), NULL);
	line = ((i = 0), combine_and_extract_line(buffer, i));
	buffer->str = (free(buffer->str), NULL);
	buffer->str_capacity = (buffer->str_len = 0, 0);
	buffer->read_index = 0;
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
