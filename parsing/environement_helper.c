/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:54:08 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 03:56:12 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*safe_strdup_value(char *value_part)
{
	if (value_part)
		return (ft_strdup(value_part));
	return (ft_strdup(""));
}

t_env	*new_env_node(char *env_str)
{
	t_env	*node;
	char	**str;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	str = ft_split(env_str, '=');
	if (!str)
	{
		free(node);
		return (NULL);
	}
	node->key = ft_strdup(str[0]);
	node->value = safe_strdup_value(str[1]);
	free_array(str);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free_env(node);
		return (NULL);
	}
	return (node);
}
