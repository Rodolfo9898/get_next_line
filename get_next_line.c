/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:35:33 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 16:15:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** read(fd, 0, 0) fails on a closed or invalid fd: drop the stash so that a
** file reopened on the same fd starts from a clean state.
*/
char	*get_next_line(int fd)
{
	static t_stash	stash;
	ssize_t			line_len;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (NULL);
	if (read(fd, 0, 0) < 0)
	{
		ft_stash_clear(&stash);
		return (NULL);
	}
	line_len = ft_read_until_newline(fd, &stash, BUFFER_SIZE);
	if (line_len <= 0)
		return (NULL);
	return (ft_extract_line(&stash, line_len));
}
