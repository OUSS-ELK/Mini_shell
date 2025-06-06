/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:20:43 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:09:39 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	indx;

	str = (char *)s;
	if (n != 0)
	{
		indx = 0;
		while (indx < n)
		{
			str[indx] = '\0';
			indx++;
		}
	}
}
