/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:57:01 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:19:23 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			loop;
	int				ret;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	loop = 0;
	ret = 0;
	while ((*str1 || *str2) && loop < n)
	{
		if (*str1 == *str2)
		{
			str1++;
			str2++;
		}
		else
		{
			ret = (*str1 - *str2);
			break ;
		}
		loop++;
	}
	return (ret);
}
