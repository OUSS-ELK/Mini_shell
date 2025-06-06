/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:33:10 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:13:27 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	int				result;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	result = 0;
	while (n--)
	{
		if (*str1 == *str2)
		{
			str1++;
			str2++;
		}
		else
			result = *str1 - *str2;
	}
	return (result);
}
