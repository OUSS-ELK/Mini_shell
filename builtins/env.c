/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:52:40 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/21 13:13:14 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_env **env)
{
	t_env	*curr;

	if (!env || !*env)
	{
		g_exit_status = 1;
		return (1);
	}
	curr = *env;
	while (curr)
	{
		if (curr->key && strcmp(curr->key, "_") == 0 && curr->value)
		{
			free(curr->value);
			curr->value = strdup("/usr/bin/env");
			if (!curr->value)
				return (g_exit_status = 1, 1);
		}
		if (curr->key && curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	g_exit_status = 0;
	return (1);
}
