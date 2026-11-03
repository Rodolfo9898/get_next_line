/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:06:12 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 16:15:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/*
** One stash per fd. fds outside [0, FD_LIMIT) are rejected before the
** array is indexed. read(fd, 0, 0) fails on a closed or invalid fd: drop
** that fd's stash so a file reopened on the same fd starts clean.
*/
char	*get_next_line(int fd)
{
	static t_stash	stash[FD_LIMIT];
	ssize_t			line_len;

	if (fd < 0 || fd >= FD_LIMIT || BUFFER_SIZE <= 0
		|| BUFFER_SIZE >= INT_MAX)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		ft_stash_clear(&stash[fd]);
		return (NULL);
	}
	line_len = ft_read_until_newline(fd, &stash[fd],
			BUFFER_SIZE);
	if (line_len <= 0)
		return (NULL);
	return (ft_extract_line(&stash[fd], line_len));
}
