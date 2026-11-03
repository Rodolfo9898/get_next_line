/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:04:40 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 16:15:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/*
** get_next_line() accepts the fds 0 to FD_LIMIT - 1. macOS defines OPEN_MAX
** (10240) in <limits.h>; glibc leaves it undefined because the Linux limit
** is set at run time, so fall back to the usual Linux default of 1024.
*/
# ifdef OPEN_MAX
#  define FD_LIMIT OPEN_MAX
# else
#  define FD_LIMIT 1024
# endif

/*
** Bytes read but not returned yet: the len bytes starting at data + start.
** cap is the size of the data allocation.
*/
typedef struct s_stash
{
	char	*data;
	size_t	start;
	size_t	len;
	size_t	cap;
}	t_stash;

void	ft_stash_clear(t_stash *stash);
int		ft_stash_reserve(t_stash *stash, size_t extra);
char	*ft_extract_line(t_stash *stash, size_t line_len);
ssize_t	ft_read_until_newline(int fd, t_stash *stash, size_t read_size);
char	*get_next_line(int fd);

#endif
