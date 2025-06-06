/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:57:25 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:19:39 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	x;

	if (!haystack && !len)
		return (NULL);
	if (!*needle)
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		x = 0;
		while (i + x < len && needle[x] && haystack[i + x])
		{
			if (haystack[i + x] != needle[x])
				break ;
			x++;
		}
		if (!needle[x])
			return ((char *)haystack + i);
		i++;
	}
	return (NULL);
}
