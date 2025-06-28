/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skeita <skeita@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:13:18 by skeita            #+#    #+#             */
/*   Updated: 2025/05/11 17:00:37 by skeita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

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
	int						fd;
	char					*str;
	char					not_read_str[BUFFER_SIZE];
	size_t					str_len;
	size_t					str_capacity;
	size_t					read_index;
	size_t					not_read_len;
	struct s_file_manager	*next;
}	t_file_manager;

char			*get_next_line(int fd);
t_file_manager	*init_file_manager_buffer(int fd);
t_file_manager	*find_or_create_buffer(t_file_manager **buffer_list, int fd);
void			remove_buffer_list(t_file_manager **buffer_list, int fd);
int				expand_buffer(t_file_manager *buffer, size_t additional_size);

char			*extract_line_from_not_read(t_file_manager *buffer, size_t i);
char			*combine_and_extract_line(t_file_manager *buffer, size_t i);
int				read_file(int fd, t_file_manager *buffer, size_t i);
void			update_remaining_data(t_file_manager *buffer,
					size_t str_line_len);
bool			exit_line_break(char *str, size_t str_length);

#endif 