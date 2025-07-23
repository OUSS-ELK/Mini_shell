/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 03:59:11 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 03:59:12 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*increment_shlvl(char *env_var)
{
	char	*lvl_str;
	int		lvl;
	char	*new_lvl_str;
	char	*new_shlvl;
	t_env	*node;

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

t_env	*handle_special_env(char *env_var, int *skip)
{
	if (ft_strncmp(env_var, "OLDPWD=", 7) == 0)
	{
		*skip = 1;
		return (NULL);
	}
	if (ft_strncmp(env_var, "SHLVL=", 6) == 0)
		return (increment_shlvl(env_var));
	return (NULL);
}

static int	handle_env_iteration(char **env, int i, t_env **curr, t_env **top)
{
	int		skip;
	t_env	*new_node;

	skip = 0;
	new_node = handle_special_env(env[i], &skip);
	if (skip)
		return (1);
	if (!new_node)
		new_node = new_env_node(env[i]);
	if (!new_node)
	{
		free_env(*top);
		return (-1);
	}
	(*curr)->next = new_node;
	*curr = new_node;
	return (0);
}

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	int		i;
	int		status;

	if (!env)
		return (NULL);
	top = new_env_node(env[0]);
	if (!top)
		return (NULL);
	curr = top;
	i = 1;
	while (env[i])
	{
		status = handle_env_iteration(env, i, &curr, &top);
		if (status == -1)
			return (NULL);
		if (status == 1)
		{
			i++;
			continue ;
		}
		i++;
	}
	return (top);
}

char	*ft_getenv(char *key, t_env *env)
{
	int	key_len;

	key_len = ft_strlen(key);
	while (env != NULL)
	{
		if (ft_strncmp(key, env->key, key_len) == 0
			&& key_len == (int)ft_strlen(env->key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
