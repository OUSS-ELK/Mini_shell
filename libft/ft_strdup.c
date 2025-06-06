/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:17 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:16:06 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s1);
	i = 0;
	s = (char *)malloc(sizeof(char) * s_len + 1);
	if (!s)
		return (NULL);
	while (i < s_len)
	{
		s[i] = s1[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}
