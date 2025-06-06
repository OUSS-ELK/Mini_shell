/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:58:31 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:20:19 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_checkset(char const *set, char c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	s_ln;
	size_t	start;
	int		i;

	if (!s1 || !set)
		return (NULL);
	if (!*s1 || !*set)
		return (ft_strdup(s1));
	start = 0;
	while (s1[start] && ft_checkset(set, s1[start]) == 1)
		start++;
	s_ln = ft_strlen(s1);
	while (s_ln > start && ft_checkset(set, s1[s_ln - 1]))
		s_ln--;
	str = (char *)malloc(sizeof(char) * (s_ln - start + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (start < s_ln)
		str[i++] = s1[start++];
	str[i] = '\0';
	return (str);
}
