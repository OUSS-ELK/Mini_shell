/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:47 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 05:28:17 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (str[1])
		node->value = ft_strdup(str[1]);
	else
		node->value = ft_strdup("");
	free_array(str);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free_env(node);
		return (NULL);
	}
	return (node);
}

t_env	*handle_special_env(char *env_var, int *skip)
{
	char	*lvl_str;
	int		lvl;
	char	*new_lvl_str;
	char	*new_shlvl;
	t_env	*node;

	if (ft_strncmp(env_var, "OLDPWD=", 7) == 0)
		return (*skip = 1, NULL);
	if (ft_strncmp(env_var, "SHLVL=", 6) == 0)
	{
		lvl_str = env_var + 6;
		lvl = ft_atoi(lvl_str);
		new_lvl_str = ft_itoa(lvl + 1);
		if (!new_lvl_str)
			return (NULL);
		new_shlvl = ft_strjoin("SHLVL=", new_lvl_str);
		free(new_lvl_str);
		if (!new_shlvl)
			return (NULL);
		node = new_env_node(new_shlvl);
		free(new_shlvl);
		return (node);
	}
	return (NULL);
}

static t_env	*init_env_list(char **env)
{
	if (!env || !env[0])
		return (NULL);
	return (new_env_node(env[0]));
}

int	skip_line(int *i, int skip)
{
	if (skip)
	{
		(*i)++;
		return (1);
	}
	return (0);
}

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	t_env	*new_node;
	int		i;
	int		skip;

	top = init_env_list(env);
	if (!top)
		return (NULL);
	curr = top;
	i = 0;
	while (env[++i])
	{
		skip = 0;
		new_node = handle_special_env(env[i], &skip);
		if (skip_line(&i, skip))
			continue ;
		if (!new_node)
			new_node = new_env_node(env[i]);
		if (!new_node)
			return (free_env(top), NULL);
		curr->next = new_node;
		curr = new_node;
	}
	return (top);
}
