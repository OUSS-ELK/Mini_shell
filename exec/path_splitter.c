/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_splitter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:27:11 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/20 10:27:12 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_t(const char *str, char sep)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str && *str == sep)
			str++;
		if (!*str)
			break ;
		if (*str == '{')
		{
			while (*str && *str != '}')
				str++;
			if (*str == '}')
				str++;
		}
		else
		{
			while (*str && *str != sep && *str != '{')
				str++;
		}
		count++;
	}
	return (count);
}

static void	get_next_t(char **str, char **start, int *len, char sep)
{
	char	*end;

	while (**str && **str == sep)
		(*str)++;
	if (!**str)
		return ;
	*start = *str;
	if (**str == '{')
	{
		while (**str && **str != '}')
			(*str)++;
		if (**str == '}')
			(*str)++;
		end = *str;
	}
	else
	{
		while (**str && **str != sep)
			(*str)++;
		end = *str;
	}
	*len = end - *start;
}

static void	copy_t(char **dst, int j, const char *start, int len)
{
	int	i;

	dst[j] = malloc(len + 1);
	if (!dst[j])
		return ;
	i = 0;
	while (i < len)
		dst[j][i++] = *start++;
	dst[j][i] = '\0';
}

static void	free_t(char **tokens, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
		free(tokens[i++]);
	free(tokens);
}

char	**split_t(const char *str, char sep)
{
	int		word_count;
	char	**tokens;
	char	*start;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	word_count = count_t(str, sep);
	tokens = malloc(sizeof(char *) * (word_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		get_next_t((char **)&str, &start, &len, sep);
		tokens[i] = malloc(len + 1);
		if (!tokens[i])
			return (free_t(tokens, i), NULL);
		copy_t(tokens, i, start, len);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
