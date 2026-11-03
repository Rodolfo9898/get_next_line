/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:32:03 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 16:15:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
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

void	ft_stash_clear(t_stash *stash);
int		ft_stash_reserve(t_stash *stash, size_t extra);
char	*ft_extract_line(t_stash *stash, size_t line_len);
ssize_t	ft_read_until_newline(int fd, t_stash *stash, size_t read_size);
char	*get_next_line(int fd);

#endif
