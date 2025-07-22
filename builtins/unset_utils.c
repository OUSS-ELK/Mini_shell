/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:01:16 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 09:01:25 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	remove_env_node(t_env **head, t_env *target)
{
	t_env	*prev;
	t_env	*curr;

	if (!head || !*head || !target)
		return (1);
	prev = NULL;
	curr = *head;
	while (curr && curr != target)
	{
		prev = curr;
		curr = curr->next;
	}
	if (!curr)
		return (1);
	if (!prev)
		*head = curr->next;
	else
		prev->next = curr->next;
	if (curr->key)
		free(curr->key);
	if (curr->value)
		free(curr->value);
	free(curr);
	return (1);
}

t_env	*envnew(char *key, char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	envadd_back(t_env **env, t_env *new)
{
	t_env	*current;

	if (!env || !new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new;
}

int	set_env(t_env **env)
{
	char	*cwd;
	char	buffer[1024];

	if (!env || *env)
		return (0);
	if (getcwd(buffer, sizeof(buffer)) != NULL)
		cwd = ft_strdup(buffer);
	else
		cwd = ft_strdup("/Users/bel-abde");
	envadd_back(env, envnew(ft_strdup("OLDPWD"), NULL));
	envadd_back(env, envnew(ft_strdup("PWD"), cwd));
	envadd_back(env, envnew(ft_strdup("SHLVL"), ft_strdup("1")));
	envadd_back(env, envnew(ft_strdup("_"), ft_strdup("/usr/bin/env")));
	return (1);
}
