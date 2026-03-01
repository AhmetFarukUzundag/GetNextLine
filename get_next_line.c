/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzundag <auzundag@student.42istanbul.com.tr  + +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:25:52 by auzundag          #+#    #+#             */
/*   Updated: 2026/03/01 07:16:42 by auzundag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*clean_stash(char *stash)
{
	int	i;
	int	j;

	if (!stash)
		return (NULL);
	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	while (stash && stash[i])
	{
		stash[j] = stash[i];
		i++;
		j++;
	}
	stash[j] = '\0';
	return (stash);
}

static char	*line_handling(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] && stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*free_stash(char **stash)
{
	free(*stash);
	*stash = NULL;
	return (NULL);
}

static char	*read_file_and_join(char **stash, int fd)
{
	char	*buffer;
	char	*new_stash;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(*stash, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buffer), NULL);
		buffer[bytes_read] = '\0';
		new_stash = ft_strjoin(*stash, buffer);
		if (!new_stash)
			return (free(buffer), NULL);
		free(*stash);
		*stash = new_stash;
	}
	free(buffer);
	return (*stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
	{
		stash = ft_calloc(1, 1);
		if (!stash)
			return (NULL);
	}
	if (!read_file_and_join(&stash, fd))
		return (free_stash(&stash));
	line = line_handling(stash);
	if (!line)
		return (free_stash(&stash));
	if (line[0] == '\0')
		return (free(line), free_stash(&stash));
	stash = clean_stash(stash);
	return (line);
}
