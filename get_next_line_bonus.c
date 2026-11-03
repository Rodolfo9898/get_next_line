/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:06:12 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 20:49:24 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/*
** Returns the stash of fd, adding an empty one at the head of the list when
** fd has none yet. Returns NULL if that malloc fails.
*/
static t_stash	*ft_get_stash(t_stash **list, int fd)
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
static void	ft_remove_stash(t_stash **list, int fd)
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
	ft_stash_clear(node);
	free(node);
}

/*
** One stash per fd, kept in a list so that any fd the system hands out can
** be read. A node only lives while its fd has bytes stored: it is removed as
** soon as its buffer is freed (end of file, read error, closed fd), so a
** file reopened on the same fd starts clean. After a malloc error in
** ft_extract_line() the buffer is kept, and so is the node.
*/
char	*get_next_line(int fd)
{
	static t_stash	*list;
	t_stash			*stash;
	ssize_t			line_len;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		ft_remove_stash(&list, fd);
		return (NULL);
	}
	stash = ft_get_stash(&list, fd);
	if (!stash)
		return (NULL);
	line = NULL;
	line_len = ft_read_until_newline(fd, stash, BUFFER_SIZE);
	if (line_len == 0)
		ft_stash_clear(stash);
	if (line_len > 0)
		line = ft_extract_line(stash, line_len);
	if (!stash->data)
		ft_remove_stash(&list, fd);
	return (line);
}
