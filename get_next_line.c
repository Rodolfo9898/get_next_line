/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:35:33 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 14:34:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** Reads BUFFER_SIZE bytes at a time and appends them to the stash until the
** stash holds a '\n' or read() reaches EOF. Only the freshly read chunk is
** searched for '\n', since the old part of the stash is known to have none.
** Takes ownership of the stash: on any error it is freed and NULL returned.
*/
char	*ft_read_file(int fd, char *stash)
{
	char	*buffer;
	char	*newline;
	ssize_t	bytes_read;

	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (free(stash), NULL);
	bytes_read = 1;
	newline = ft_strchr(stash, '\n');
	while (!newline && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			break ;
		buffer[bytes_read] = '\0';
		stash = ft_strjoin(stash, buffer);
		if (!stash)
			break ;
		newline = ft_strchr(buffer, '\n');
	}
	free(buffer);
	if (bytes_read < 0)
		return (free(stash), NULL);
	return (stash);
}

/*
** read(fd, 0, 0) fails on a closed or invalid fd: drop the stash so that a
** file reopened on the same fd starts from a clean state.
*/
char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = ft_read_file(fd, stash);
	if (!stash)
		return (NULL);
	line = ft_isolate_single_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = ft_get_rest_of_lines(stash);
	return (line);
}
