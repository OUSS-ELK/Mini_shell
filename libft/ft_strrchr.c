/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:58:03 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:20:02 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	size_t	l_st;

	str = (char *)s;
	l_st = ft_strlen(str);
	while (l_st > 0)
	{
		if (str[l_st] == (char)c)
			return (str + l_st);
		l_st--;
	}
	if (str[l_st] == (char)c)
		return (str + l_st);
	return (NULL);
}
