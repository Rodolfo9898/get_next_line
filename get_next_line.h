/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:32:03 by rperez-t          #+#    #+#             */
/*   Updated: 2023/12/12 16:18:20 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# define CHARSIZE 1

char	*get_next_line(int fd);
char	*ft_read_file(int fd, char *file);
char	*ft_strchr(char *s, int c);
char	*ft_strjoin(char *str, char *joinee);
size_t	ft_strlen(char *str);
char	*ft_isolate_single_line(char *file);
char	*ft_get_rest_of_lines(char *file);

#endif