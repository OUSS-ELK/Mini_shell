/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:33:05 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:10:13 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*als;
	size_t	i;

	i = 0;
	als = (char *)malloc(size * count);
	if (!als)
		return (NULL);
	while (i < count * size)
	{
		als[i] = '\0';
		i++;
	}
	return (als);
}
