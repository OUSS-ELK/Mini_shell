/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:14:20 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/11 21:59:44 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_key(char *str)
{
	int		i;
	char	*key;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
		i++;
	return (ft_substr(str, 0, i));
}

char	*get_value(char *str)
{
	char	*tmp;

	tmp = ft_strchr(str, '=');
	if (!tmp || !*(tmp + 1)) // no value or '=' is the last char
		return (NULL);
	return (ft_strdup(tmp + 1));
}

void	add_to_env(t_env **env_lst, char *key, char *value)
{
	t_env	*new;
	t_env	*tail;

	if (!env_lst || !key)
		return ;
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = key;
	new->value = value;
	new->next = NULL;
	if (!*env_lst)
	{
		*env_lst = new;
		return ;
	}
	tail = *env_lst;
	while (tail->next)
		tail = tail->next;
	tail->next = new;
}

void	append_env_value(t_env *tmp, char *value)
{
	char	*joined;

	if (!tmp || !value)
		return ;
	joined = ft_strjoin(tmp->value, value);
	free(tmp->value);
	free(value);
	tmp->value = joined;
}

void	replace_env_value(t_env *tmp, char *value)
{
	if (!tmp)
		return ;
	free(tmp->value);
	if (value)
		tmp->value = value;
	else
		tmp->value = NULL;
}
