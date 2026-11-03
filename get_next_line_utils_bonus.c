/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:05:04 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 14:34:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(char *str)
{
	size_t	count;

	count = 0;
	if (!str)
		return (0);
	while (str[count] != '\0')
		count++;
	return (count);
}

char	*ft_strchr(char *str, int c)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == (char)c)
			return (str);
		str++;
	}
	if ((char)c == '\0')
		return (str);
	return (NULL);
}

/*
** Returns stash + buffer in a new allocation. The old stash is always freed,
** also when malloc fails, so the caller never has to free it again.
*/
char	*ft_strjoin(char *stash, char *buffer)
{
	char	*joined;
	size_t	stash_len;
	size_t	i;

	stash_len = ft_strlen(stash);
	joined = malloc(stash_len + ft_strlen(buffer) + 1);
	if (!joined)
	{
		free(stash);
		return (NULL);
	}
	i = 0;
	while (i < stash_len)
	{
		joined[i] = stash[i];
		i++;
	}
	while (*buffer)
		joined[i++] = *buffer++;
	joined[i] = '\0';
	free(stash);
	return (joined);
}

char	*ft_isolate_single_line(char *stash)
{
	char	*line;
	size_t	line_len;
	size_t	i;

	if (!stash[0])
		return (NULL);
	line_len = 0;
	while (stash[line_len] && stash[line_len] != '\n')
		line_len++;
	if (stash[line_len] == '\n')
		line_len++;
	line = malloc(line_len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < line_len)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

/*
** Keeps what follows the first '\n' for the next call. Always frees the old
** stash; returns NULL when nothing is left or when malloc fails.
*/
char	*ft_get_rest_of_lines(char *stash)
{
	char	*rest;
	size_t	start;
	size_t	i;

	start = 0;
	while (stash[start] && stash[start] != '\n')
		start++;
	if (!stash[start] || !stash[start + 1])
	{
		free(stash);
		return (NULL);
	}
	start++;
	rest = malloc(ft_strlen(stash + start) + 1);
	if (rest)
	{
		i = 0;
		while (stash[start])
			rest[i++] = stash[start++];
		rest[i] = '\0';
	}
	free(stash);
	return (rest);
}
