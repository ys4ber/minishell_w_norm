/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkazaz <lkazaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:35:18 by lkazaz            #+#    #+#             */
/*   Updated: 2024/03/11 07:16:03 by lkazaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_line(int fd, char *str)
{
	char	*buff;
	int		i;

	i = 1;
	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	while (!ft_strchr(str, '\n') && i != 0)
	{
		i = read(fd, buff, BUFFER_SIZE);
		if (i == -1)
		{
			free(buff);
			free(str);
			return (NULL);
		}
		buff[i] = '\0';
		str = ft_strjoin(str, buff);
		if (!str)
			return (NULL);
	}
	free(buff);
	return (str);
}

char	*ft_frst_line(char *str)
{
	char	*src;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str[0])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i])
		i++;
	src = malloc((i + 1) * sizeof(char));
	if (!src)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
		src[j++] = str[i++];
	src[j] = '\0';
	return (src);
}

char	*ft_last_line(char *str)
{
	char	*src;
	int		j;
	size_t	i;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\0')
	{
		free(str);
		return (NULL);
	}
	src = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if (!src)
		return (NULL);
	if (str[i] == '\n')
		i++;
	while (str[i] != '\0')
		src[j++] = str[i++];
	src[j] = '\0';
	free(str);
	return (src);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*src;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	str = get_line(fd, str);
	if (!str)
		return (NULL);
	src = ft_frst_line(str);
	str = ft_last_line(str);
	return (src);
}
