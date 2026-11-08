/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:06:12 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 22:58:06 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

#if BONUS_MODE

/*
** Returns the stash of fd, adding an empty one at the head of the list when
** fd has none yet. Returns NULL if that malloc fails.
*/
static t_stash	*gnl_get_stash(t_stash **list, int fd)
{
	t_stash	*node;

	node = *list;
	while (node && node->fd != fd)
		node = node->next;
	if (node)
		return (node);
	node = malloc(sizeof(t_stash));
	if (!node)
		return (NULL);
	node->data = NULL;
	node->start = 0;
	node->len = 0;
	node->cap = 0;
	node->fd = fd;
	node->next = *list;
	*list = node;
	return (node);
}

/*
** Unlinks the stash of fd, if any, and frees its buffer and node.
*/
static void	gnl_remove_stash(t_stash **list, int fd)
{
	t_stash	**link;
	t_stash	*node;

	link = list;
	while (*link && (*link)->fd != fd)
		link = &(*link)->next;
	if (!*link)
		return ;
	node = *link;
	*link = node->next;
	gnl_stash_clear(node);
	free(node);
}

/*
** BONUS ENTRY POINT, called by get_next_line() when BONUS_MODE is 1, after
** it checked fd and BUFFER_SIZE. *list is the head of the stashes: one per
** fd, so that any fd the system hands out can be read. A node only lives
** while its fd has bytes stored: it is removed as soon as its buffer is
** freed (end of file, read error, closed fd), so a file reopened on the same
** fd starts clean. After a malloc error in gnl_extract_line() the buffer is
** kept, and so is the node.
*/
char	*gnl_bonus_get_next_line(int fd, t_stash **list)
{
	t_stash	*stash;
	ssize_t	line_len;
	char	*line;

	if (read(fd, 0, 0) < 0)
	{
		gnl_remove_stash(list, fd);
		return (NULL);
	}
	stash = gnl_get_stash(list, fd);
	if (!stash)
		return (NULL);
	line = NULL;
	line_len = gnl_read_until_newline(fd, stash, BUFFER_SIZE);
	if (line_len == 0)
		gnl_stash_clear(stash);
	if (line_len > 0)
		line = gnl_extract_line(stash, line_len);
	if (!stash->data)
		gnl_remove_stash(list, fd);
	return (line);
}

#endif
