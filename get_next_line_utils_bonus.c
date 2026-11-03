/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:05:04 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 16:15:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	ft_stash_clear(t_stash *stash)
{
	free(stash->data);
	stash->data = NULL;
	stash->start = 0;
	stash->len = 0;
	stash->cap = 0;
}

/*
** Makes room for extra bytes after the stored ones. When the space left is
** too small, the stored bytes move to the front of the block if they fit
** there, else to a new block twice as large as needed, so each byte is
** copied only a constant number of times on average. Copying forwards is
** safe when the block is reused, because the bytes only move to the left.
** On malloc failure the stash is left untouched and 0 is returned.
*/
int	ft_stash_reserve(t_stash *stash, size_t extra)
{
	char	*new_data;
	size_t	i;

	if (stash->start + stash->len + extra <= stash->cap)
		return (1);
	new_data = stash->data;
	if (stash->len + extra > stash->cap)
		new_data = malloc(2 * (stash->len + extra));
	if (!new_data)
		return (0);
	i = 0;
	while (i < stash->len)
	{
		new_data[i] = stash->data[stash->start + i];
		i++;
	}
	if (new_data != stash->data)
	{
		free(stash->data);
		stash->cap = 2 * (stash->len + extra);
	}
	stash->data = new_data;
	stash->start = 0;
	return (1);
}

/*
** Returns the first line_len stored bytes as a new string and drops them
** from the stash. On malloc failure the stash is kept, so no data is lost
** and a later call returns the same line.
*/
char	*ft_extract_line(t_stash *stash, size_t line_len)
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
** line (0 when nothing is left; get_next_line() then frees the stash), or
** -1 on error. A read error drops the stash, because the fd may be
** unusable. A malloc error keeps it: the fd position is past the stored
** bytes, so dropping them would lose data.
** read_size comes from get_next_line(), which checks BUFFER_SIZE first, so
** a negative BUFFER_SIZE still compiles.
*/
ssize_t	ft_read_until_newline(int fd, t_stash *stash, size_t read_size)
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
		{
			ft_stash_clear(stash);
			return (-1);
		}
		stash->len += bytes_read;
	}
	return (stash->len);
}
