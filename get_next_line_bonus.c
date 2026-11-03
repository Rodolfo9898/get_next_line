/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:06:12 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 14:34:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	ft_stash_clear(t_stash *stash)
{
	free(stash->data);
	stash->data = NULL;
	stash->start = 0;
	stash->len = 0;
	stash->cap = 0;
}

/*
** Makes room for extra bytes after the stored ones. When the space left is
** too small, the stored bytes move to a new block twice as large as needed,
** so each byte is copied only a constant number of times on average.
** On malloc failure the stash is left untouched and 0 is returned.
*/
static int	ft_stash_reserve(t_stash *stash, size_t extra)
{
	char	*new_data;
	size_t	new_cap;
	size_t	i;

	if (stash->start + stash->len + extra <= stash->cap)
		return (1);
	new_cap = 2 * (stash->len + extra);
	new_data = malloc(new_cap);
	if (!new_data)
		return (0);
	i = 0;
	while (i < stash->len)
	{
		new_data[i] = stash->data[stash->start + i];
		i++;
	}
	free(stash->data);
	stash->data = new_data;
	stash->start = 0;
	stash->cap = new_cap;
	return (1);
}

/*
** Returns the first line_len stored bytes as a new string and drops them
** from the stash. On malloc failure the stash is kept, so no data is lost.
*/
static char	*ft_extract_line(t_stash *stash, size_t line_len)
{
	char	*line;
	size_t	i;

	line = malloc(line_len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < line_len)
	{
		line[i] = stash->data[stash->start + i];
		i++;
	}
	line[i] = '\0';
	stash->start += line_len;
	stash->len -= line_len;
	if (stash->len == 0)
		ft_stash_clear(stash);
	return (line);
}

/*
** Searches the stash for a '\n' and, while there is none and read() has not
** reached end of file, appends one BUFFER_SIZE-byte block. newline is never
** reset, so each byte is searched only once. Returns the length of the next
** line (0 when nothing is left), or -1 on error. A read error drops the
** stash; a malloc error keeps it. read_size comes from get_next_line(),
** which checks BUFFER_SIZE first, so a negative BUFFER_SIZE still compiles.
*/
static ssize_t	ft_read_until_newline(int fd, t_stash *stash, size_t read_size)
{
	size_t	newline;
	ssize_t	bytes_read;

	newline = 0;
	bytes_read = 1;
	while (bytes_read > 0)
	{
		while (newline < stash->len
			&& stash->data[stash->start + newline] != '\n')
			newline++;
		if (newline < stash->len)
			return (newline + 1);
		if (!ft_stash_reserve(stash, read_size))
			return (-1);
		bytes_read = read(fd, stash->data + stash->start + stash->len,
				read_size);
		if (bytes_read < 0)
			return (ft_stash_clear(stash), -1);
		stash->len += bytes_read;
	}
	if (stash->len == 0)
		ft_stash_clear(stash);
	return (stash->len);
}

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
