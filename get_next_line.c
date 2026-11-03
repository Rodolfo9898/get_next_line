/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:35:33 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 22:08:04 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** The stash only ever holds bytes from stash.fd. read(fd, 0, 0) fails on a
** closed or invalid fd: if that is stash.fd, drop the bytes so that a file
** reopened on the same fd starts from a clean state; a bad call on another
** fd keeps them. A call on another, valid fd drops them before reading, so
** lines from two fds are never mixed (reading several fds is the bonus).
*/
char	*get_next_line(int fd)
{
	static t_stash	stash;
	ssize_t			line_len;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		if (fd == stash.fd)
			ft_stash_clear(&stash);
		return (NULL);
	}
	if (fd != stash.fd)
		ft_stash_clear(&stash);
	stash.fd = fd;
	line_len = ft_read_until_newline(fd, &stash, BUFFER_SIZE);
	if (line_len == 0)
		ft_stash_clear(&stash);
	if (line_len <= 0)
		return (NULL);
	return (ft_extract_line(&stash, line_len));
}
