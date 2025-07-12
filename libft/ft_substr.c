/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:58:50 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 17:22:27 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len || len == 0)
		return (ft_strdup(""));
	s = s + start;
	s_len = ft_strlen(s);
	if (s_len > len)
		s_len = len;
	sub = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!sub)
		return (NULL);
	sub = ft_memcpy(sub, s, s_len);
	sub[s_len] = '\0'; 
	return (sub);
}
