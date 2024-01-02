/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:14:32 by rperez-t          #+#    #+#             */
/*   Updated: 2023/12/18 09:08:28 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	int		idx;
	char	character;
	int		size;
	char	*address;

	idx = 0;
	character = c;
	if (!str)
		return (0);
	size = ft_strlen(str);
	address = &str[size];
	if (character == '\0')
		return (address);
	while (str[idx] != '\0')
	{
		address = &str[idx];
		if (str[idx] == character)
			return (address);
		idx++;
	}
	return (0);
}

char	*ft_strjoin(char *str, char *joinee)
{
	int		vals[3];
	char	*res;

	if (!str)
	{
		str = malloc(CHARSIZE);
		str[0] = '\0';
	}
	if (!joinee)
		return (NULL);
	vals[2] = ft_strlen(str) + ft_strlen(joinee) + 1;
	res = malloc(vals[2]);
	if (res == NULL)
		return (NULL);
	vals[0] = -1;
	vals[1] = 0;
	if (str)
		while (str[++vals[0]] != '\0')
			res[vals[0]] = str[vals[0]];
	while (joinee[vals[1]] != '\0')
		res[vals[0]++] = joinee[vals[1]++];
	vals[2]--;
	res[vals[2]] = '\0';
	free(str);
	return (res);
}

char	*ft_isolate_single_line(char *file)
{
	int		vals[2];
	char	*line;

	vals[0] = 0;
	if (!file[vals[0]])
		return (NULL);
	while (file[vals[0]] && file[vals[0]] != '\n')
		vals[0]++;
	vals[1] = vals[0] + 2;
	line = malloc(vals[1]);
	if (!line)
		return (NULL);
	vals[0] = 0;
	while (file[vals[0]] && file[vals[0]] != '\n')
	{
		line[vals[0]] = file[vals[0]];
		vals[0]++;
	}
	if (file[vals[0]] == '\n')
	{
		line[vals[0]] = file[vals[0]];
		vals[0]++;
	}
	line[vals[0]] = '\0';
	return (line);
}

char	*ft_get_rest_of_lines(char *file)
{
	int		vals[3];
	char	*rest_of_lines;

	vals[0] = 0;
	while (file[vals[0]] && file[vals[0]] != '\n')
		vals[0]++;
	if (!file[vals[0]])
	{
		free(file);
		return (NULL);
	}
	vals[2] = ft_strlen(file) - vals[0] + 1;
	rest_of_lines = malloc(vals[2]);
	if (!rest_of_lines)
		return (NULL);
	vals[0]++;
	vals[1] = 0;
	while (file[vals[0]])
		rest_of_lines[vals[1]++] = file[vals[0]++];
	rest_of_lines[vals[1]] = '\0';
	free(file);
	return (rest_of_lines);
}
