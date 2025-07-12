/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:23:16 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/12 00:03:01 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	gnl_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*gnl_strjoin(char *stash, char *buffer)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	j = 0;
	if (!stash)
		return (gnl_strdup(buffer));
	ptr = malloc(gnl_strlen(stash) + gnl_strlen(buffer) + 1);
	if (!ptr)
		return (NULL);
	while (stash[i])
	{
		ptr[i] = stash[i];
		i++;
	}
	while (buffer[j])
	{
		ptr[i] = buffer[j];
		i++;
		j++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*gnl_strchr(char *str, int c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] || (char)c == '\0')
	{
		if (str[i] == (char)c)
			return ((char *)str + i);
		i++;
	}
	return (NULL);
}

void	*gnl_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;
	size_t			i;
	size_t			l;

	i = 0;
	l = count * size ;
	if (size && count != (l / size))
		return (NULL);
	ptr = malloc(l);
	if (!ptr)
		return (NULL);
	while (i < l)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}

char	*gnl_strdup(char *s1)
{
	size_t	i;
	char	*ptr;

	ptr = (char *) malloc(sizeof(char) * (gnl_strlen(s1) + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
