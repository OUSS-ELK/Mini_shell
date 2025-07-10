/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oussama-elk <oussama-elk@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:41:10 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/10 13:38:55 by oussama-elk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	word;

	while (*s && *s == c)
		s++;
	word = 0;
	while (*s)
	{
		if (*s && *s != c && (*(s + 1) == c || *(s + 1) == '\0'))
			word++;
		s++;
	}
	return (word);
}

static void	*ft_free(char **src)
{
	char	**new;

	new = src;
	while (*src)
	{
		free(*src);
		src++;
	}
	free(new);
	return (NULL);
}

static char	**ft_copyword(char **spl, char const *s, char c, size_t count)
{
	size_t			i;
	unsigned int	x;

	i = 0;
	while (i < count)
	{
		x = 0;
		while (*s && *s == c)
			s++;
		while (s[x] && s[x] != c)
			x++;
		spl[i] = ft_substr(s, 0, x);
		if (!spl[i])
			return (ft_free(spl));
		while (*s && *s != c)
			s++;
		i++;
	}
	spl[count] = NULL;
	return (spl);
}

char	**ft_split(char const *s, char c)
{
	char	**spl;
	size_t	count;

	count = 0;
	if (!s)
		return (NULL);
	count = ft_wordcount(s, c);
	spl = (char **)malloc(sizeof(char *) * (count + 1));
	if (!spl)
		return (NULL);
	spl = ft_copyword(spl, s, c, count);
	return (spl);
}
