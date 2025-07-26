/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 08:48:03 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 08:59:06 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	export_add_node(t_env **env_lst, char **splitted)
{
	t_env	*new;
	t_env	*head;

	if (!env_lst || !splitted)
		return (0);
	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = splitted[0];
	new->value = splitted[1];
	new->next = NULL;
	if (!(*env_lst))
		*env_lst = new;
	else
	{
		head = *env_lst;
		while (head->next)
			head = head->next;
		head->next = new;
	}
	return (1);
}
