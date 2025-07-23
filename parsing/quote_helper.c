/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:45:18 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 03:46:40 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*f_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	*f_strjoin_char(char *s, char c)
{
	char	str[2];

	str[0] = c;
	str[1] = 0;
	return (f_strjoin_free(s, str));
}

char	*exit_status(char *result, int *i)
{
	char	*tmp;

	tmp = ft_itoa(g_exit_status);
	result = f_strjoin_free(result, tmp);
	free(tmp);
	(*i)++;
	return (result);
}
