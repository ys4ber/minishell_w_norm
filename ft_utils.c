/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaber <ysaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:21:57 by ysaber            #+#    #+#             */
/*   Updated: 2024/03/12 00:36:08 by ysaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	if (!s1 || !s2)
// 		return (-1);
// 	while ((*s1 != '\0' || *s2 != '\0') && n > 0)
// 	{
// 		if (*s1 != *s2)
// 		{
// 			return ((unsigned char)*s1 - (unsigned char)*s2);
// 		}
// 		s1++;
// 		s2++;
// 		n--;
// 	}
// 	return (0);
// }

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (i < n && (s1[i] || s2[i]))
	{
		if (s2[i] != s1[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

int ft_strlcpy(char *dst, const char *src, int size)
{
    int i;
    
    i = 0;
    while (src[i] && i < size - 1)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    while (src[i])
        i++;
    return (i);
}


int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = ft_malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}


int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*sub;
	int		str_len;
	int		i;

	str_len = ft_strlen(s);
	if (start >= str_len)
		return (NULL);
	if (len > str_len - start)
		len = str_len - start;
	sub = ft_malloc(len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[len] = '\0';
	return (sub);
}

char	*ft_strjoin2(char *s1, char *s2, char *s3)
{
	char	*new_str;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	if (!s1)
		s1 = ft_strdup("");
	if (!s2)
		s2 = ft_strdup("");
	if (!s3)
		s3 = ft_strdup("");
	new_str = ft_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(s3) + 1));
	if (new_str == NULL)
		return (NULL);
	while (s1[i])
		new_str[k++] = s1[i++];
	while (s2[j])
		new_str[k++] = s2[j++];
	j = 0;
	while (s3[j])
		new_str[k++] = s3[j++];
	new_str[k] = '\0';
	return (new_str);
}

char *ft_last_world(char *str)
{
	int i;
	int j;
	char *last_word;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '/')
			j = i;
		i++;
	}
	if (j!=0)
		last_word = ft_substr(str, j+1, i - j);
	else
		last_word = ft_substr(str, j, i - j);
	return last_word;
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	if (!s1)
	{
		s1 = (char *)ft_malloc(1 * sizeof(char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (s1);
	str = ft_malloc(sizeof(char) * ((ft_strlen(s1) + ft_strlen(s2)) + 1));
	if (str == NULL)
		return (NULL);
	i = -1;
	j = 0;
	if (s1)
		while (s1[++i] != '\0')
			str[i] = s1[i];
	while (s2[j] != '\0')
		str[i++] = s2[j++];
	str[ft_strlen(s1) + ft_strlen(s2)] = '\0';
	return (str);
}


int	ft_args_len(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		while (needle[j] && haystack[i + j] == needle[j] && i + j < len)
			j++;
		if (needle[j] == '\0')
			return ((char *)haystack + i);
		i++;
	}
	return (NULL);
}


int ft_strlen_2d(char **str)
{
    int i = 0;
    while (str[i] != NULL)
        i++;
    return i;
}


t_list					*g_allocations = NULL;

void	*ft_malloc(size_t size)
{
	void	*ptr;
	t_list	*new_node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (ptr)
	{
		new_node = malloc(sizeof(t_list));
		
		if (new_node)
		{
			new_node->ptr = ptr;
			new_node->next = g_allocations;
			g_allocations = new_node;
		}
		else
		{
			free(ptr);
			ptr = NULL;
		}
	}
	return (ptr);
}

void	ft_free_all(void)
{
	t_list *tmp;
	while (g_allocations)
	{
		free(g_allocations->ptr);
		tmp = g_allocations;
		g_allocations = g_allocations->next;
		free(tmp);
	}
}

int ft_count_words(char *str, char c)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == c)
            count++;
        i++;
    }
    return (count + 1);
}

char **ft_split2(char *str, char c)
{
    int i;
    int j;
    int k;
    char **ret;

    i = 0;
    j = 0;
    k = 0;
    ret = ft_malloc(sizeof(char *) * (ft_count_words(str, c) + 1));
    while (str[i])
    {
        if (str[i] == c)
        {
            if (i != k)
            {
                ret[j] = ft_malloc(sizeof(char) * (i - k + 1));
                ft_strlcpy(ret[j], &str[k], i - k + 1);
                j++;
            }
            i++;
            k = i;
        }
        else if (str[i] == '\"')
        {
            i++;
            while (str[i] != '\"' && str[i])
                i++;
            if (str[i] == '\"')
                i++;
        }
        else
            i++;
    }
    if (i != k)
    {
        ret[j] = ft_malloc(sizeof(char) * (i - k + 1));
        ft_strlcpy(ret[j], &str[k], i - k + 1);
        j++;
    }
    ret[j] = NULL;
    return (ret);
}

char *ft_strtrim(char *str, char *set)
{
    int i;
    int j;
    char *ret;

    i = 0;
    j = strlen(str) - 1;
    while (str[i] && ft_strchr(set, str[i]))
        i++;
    while (j > i && ft_strchr(set, str[j]))
        j--;
    ret = ft_malloc(sizeof(char) * (j - i + 2));
    ft_strlcpy(ret, &str[i], j - i + 2);
    return (ret);
}