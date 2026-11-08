/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:35:33 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 22:58:06 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

/*
** The stash only ever holds bytes read from stash.fd. read(fd, 0, 0) tells
** whether fd can be read:
** - fd unreadable and fd == stash.fd: drop the bytes, so a file reopened
**   on the same fd starts clean;
** - fd unreadable and fd != stash.fd: keep the bytes;
** - fd readable and fd != stash.fd: drop the bytes before reading, so two
**   fds never mix (reading several fds at once is the bonus).
** stash is the only static variable of both builds: with make bonus,
** stash.next is the head of the list of stashes, one per fd, and
** gnl_bonus_get_next_line() does the work.
*/
char	*get_next_line(int fd)
{
	static t_stash	stash;
	ssize_t			line_len;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	if (BONUS_MODE)
		return (gnl_bonus_get_next_line(fd, &stash.next));
	if (read(fd, 0, 0) < 0)
	{
		if (fd == stash.fd)
			gnl_stash_clear(&stash);
		return (NULL);
	}
	if (fd != stash.fd)
		gnl_stash_clear(&stash);
	stash.fd = fd;
	line_len = gnl_read_until_newline(fd, &stash, BUFFER_SIZE);
	if (line_len == 0)
		gnl_stash_clear(&stash);
	if (line_len <= 0)
		return (NULL);
	return (gnl_extract_line(&stash, line_len));
}
