/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:04:40 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 14:34:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

/* get_next_line() accepts the fds 0 to FD_LIMIT - 1. */
# ifndef FD_LIMIT
#  define FD_LIMIT 1024
# endif

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

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

char	*get_next_line(int fd);

#endif
