/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 22:27:01 by ouelkhar          #+#    #+#             */
/*   Updated: 2024/11/30 13:12:53 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_ncount(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n = n * -1;
		count++;
	}
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static char	*ft_fill(char *s, long n, int count, int sign)
{
	s[count] = '\0';
	while (--count >= sign)
	{
		s[count] = (n % 10) + 48;
		n /= 10;
	}
	return (s);
}

char	*ft_itoa(int nbr)
{
	char	*str;
	int		n_count;
	int		sign;
	long	n;

	n = nbr;
	if (n < 0)
		sign = 1;
	else
		sign = 0;
	n_count = ft_ncount(n);
	str = (char *)malloc(n_count + 1);
	if (!str)
		return (NULL);
	if (sign == 1)
	{
		str[0] = '-';
		n = n * -1;
	}
	str = ft_fill(str, n, n_count, sign);
	return (str);
}
