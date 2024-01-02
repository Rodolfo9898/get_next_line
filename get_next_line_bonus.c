/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:06:12 by rperez-t          #+#    #+#             */
/*   Updated: 2023/12/29 13:53:27 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
			return (free(line), NULL);
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
	static char	*file[FDS_TO_OPEN];
	int			reader;

	if (fd < 0)
		reader = check_read(fd, NULL);
	else
		reader = check_read(fd, file[fd]);
	if (fd < 0 || BUFFER_SIZE <= 0 || INT_MAX <= BUFFER_SIZE || reader)
		return (0);
	file[fd] = ft_read_file(fd, file[fd]);
	if (!file[fd])
		return (NULL);
	line = ft_isolate_single_line(file[fd]);
	file[fd] = ft_get_rest_of_lines(file[fd]);
	if (line == NULL)
		return (free(line), NULL);
	return (line);
}
/*
int	main(int argc, char **argv)
{
	char	*lines[2];
	int		vals[5];

	if (argc == 4)
	{
		vals[1] = open(argv[1], O_RDONLY);
		vals[2] = open(argv[2], O_RDONLY);
		vals[0] = 0;
		while (vals[0] < atoi(argv[3]))
		{
			vals[3] = 0;
			vals[4] = 0;
			while (vals[3] < 2)
			{
				lines[vals[3]] = get_next_line(vals[++vals[4]]);
				if (!lines[vals[3]])
					break ;
				printf("%s\n", lines[vals[3]]);
				free(lines[vals[3]]);
				vals[3]++;
			}
			vals[0]++;
		}
	}
	return (close(vals[1]), close(vals[2]), 0);
}
*/