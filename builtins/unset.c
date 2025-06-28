/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:39:15 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/28 15:26:42 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	free_env_node(t_env **head, t_env *target)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *head;
	while (curr && curr != target)
	{
		prev = curr;
		curr = curr->next;
	}
	if (!prev)
		*head = curr->next;
	else
		prev->next = curr->next;
	free(curr->key);
	free(curr->value);
	free(curr);
}


static int	is_valid(const char *s, t_exec *exec)
{
	int	i;

	i = 0;
	if (!s || *s == '=' || *s == '+' || ft_isdigit(*s))
	{
		exec->exit_status = 1;
		// print error minishell unset (char *s), "not a valid identifier"
		return (0);
	}
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
		{
			exec->exit_status = 1;
			// print error minishell unset (char *s), "not a valid identifier"
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_unset(char **av, t_env **env_lst, t_exec *exec)
{
	t_env	*tmp;
	int		i;

	if (!env_lst || !av)
		return ;
	exec->exit_status = 0;
	i = 0;
	while (av[++i])
	{
		if (!is_valid(av[i], exec))
			continue;
		tmp = *env_lst;
		while (tmp && ft_strcmp(tmp->key, av[i], ft_strlen(av)) != 0)
			tmp = tmp->next;
		if (tmp)
			free_env_node(env_lst, tmp);
	}
}
