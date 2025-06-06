/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:49:37 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 17:59:49 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	s_len;
	size_t	d_len;
	size_t	d_l;

	s_len = ft_strlen(src);
	if (!dst && !dstsize)
		return (s_len);
	d_len = ft_strlen(dst);
	d_l = d_len;
	if (d_len > dstsize)
		return (s_len + dstsize);
	if (d_len < dstsize)
	{
		while ((d_l < dstsize - 1) && *src)
		{
			dst[d_l] = *src;
			d_l++;
			src++;
		}
		dst[d_l] = '\0';
	}
	return (s_len + d_len);
}
