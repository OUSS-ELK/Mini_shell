/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 02:26:50 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 02:29:24 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*token_list_to_str(t_token *token)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (token)
	{
		tmp = result;
		result = ft_strjoin(result, token->token);
		free(tmp);
		token = token->next;
	}
	return (result);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (s1);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (free(s1), free(s2), NULL);
	ft_memcpy(joined, s1, len1);
	ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	free(s1);
	return (joined);
}

void	ft_free_env_list(t_env **list)
{
	t_env	*current;
	t_env	*next;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*list = NULL;
}
