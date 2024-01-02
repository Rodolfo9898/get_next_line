/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 13:35:33 by rperez-t          #+#    #+#             */
/*   Updated: 2023/12/29 13:05:53 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_read_file(int fd, char *file)
{
	char	*line;
	char	*rest_of_file;
	int		index;

	line = malloc(BUFFER_SIZE + 1);
	if (!line)
		return (NULL);
	index = 1;
	rest_of_file = ft_strchr(file, '\n');
	while (!rest_of_file && index != 0)
	{
		index = read(fd, line, BUFFER_SIZE);
		if (index == EOF || index < 0)
		{
			free(line);
			return (NULL);
		}
		line[index] = '\0';
		file = ft_strjoin(file, line);
		rest_of_file = ft_strchr(file, '\n');
	}
	free(line);
	return (file);
}

int	check_read(int fd, char *file)
{
	int	idx;

	idx = 0;
	if (read(fd, 0, 0) < 0)
	{
		if (file)
		{
			while (file[idx] != '\0')
				file[idx++] = '\0';
		}
		return (1);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*file;
	int			reader;

	reader = check_read(fd, file);
	if (fd < 0 || BUFFER_SIZE <= 0 || INT_MAX <= BUFFER_SIZE || reader)
		return (0);
	file = ft_read_file(fd, file);
	if (!file)
		return (NULL);
	line = ft_isolate_single_line(file);
	file = ft_get_rest_of_lines(file);
	if (line == NULL)
	{
		free(line);
		return (NULL);
	}
	return (line);
}
/*
int	main(int argc, char **argv)
{
	char	*line;
	int		i;
	int		fd;
	int		max;

	if (argc == 2)
	{
		fd = open("test.txt", O_RDONLY);
		max = atoi(argv[1]);
		i = 0;
		while (i < max)
		{
			line = get_next_line(fd);
			if (!line)
				return (0);
			printf("%s\n", line);
			free(line);
			i++;
		}
		close(fd);
	}
	return (0);
}
*/