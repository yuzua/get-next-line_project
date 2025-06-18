/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skeita <skeita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:18 by skeita            #+#    #+#             */
/*   Updated: 2025/05/11 17:00:37 by skeita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# define OPEN_ERROR -1
# define READ_ERROR -1
# define FILE_TERMINATION 0

typedef struct s_file_manager
{
	char	*str;
	size_t	str_len;
	size_t	str_capacity;
	size_t	read_index;
}	t_file_manager;

char			*get_next_line(int fd);

t_file_manager	*init_file_manager_buffer(void);
int				read_file(int fd, t_file_manager *buffer, size_t buffer_size);
char			*read_str_calloc(size_t str_len);
int				expand_buffer(t_file_manager *buffer, size_t additional_size);
bool			exit_line_break(char *str, size_t str_length);
char			*read_line(t_file_manager *buffer);

#endif