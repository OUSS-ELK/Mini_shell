/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:47 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/23 03:17:05 by ouelkhar         ###   ########.fr       */
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
	node->value = ft_strdup(str[1] ? str[1] : "");
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

	/* ---------- skip OLDPWD at startup ---------- */
	if (ft_strncmp(env_var, "OLDPWD=", 7) == 0)
	{
		*skip = 1;          /* set flag, do NOT dereference like *skip++  */
		return (NULL);
	}

	/* ---------- increment SHLVL ---------- */
	if (ft_strncmp(env_var, "SHLVL=", 6) == 0)
	{
		lvl_str      = env_var + 6;           /* points after "SHLVL="     */
		lvl          = ft_atoi(lvl_str);      /* current level             */
		new_lvl_str  = ft_itoa(lvl + 1);      /* "n+1" as string           */
		if (!new_lvl_str)
			return (NULL);
		new_shlvl    = ft_strjoin("SHLVL=", new_lvl_str);
		free(new_lvl_str);
		if (!new_shlvl)
			return (NULL);
		node = new_env_node(new_shlvl);
		free(new_shlvl);
		return (node);                        /* may be NULL on malloc fail */
	}

	/* not a special var */
	return (NULL);
}

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	t_env	*new_node;
	int		i;
	int		skip;

	if (!env)
		return (NULL);
	top = new_env_node(env[0]);
	if (!top)
		return (NULL);
	curr = top;
	i = 1;
	while (env[i])
	{
		skip = 0;
		new_node = handle_special_env(env[i], &skip);
		if (skip)
		{
			i++;
			continue;
		}
		if (!new_node) // not SHLVL or OLDPWD, just a regular var
			new_node = new_env_node(env[i]);
		if (!new_node)
		{
			free_env(top);
			return (NULL);
		}
		curr->next = new_node;
		curr = new_node;
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
		if (ft_strncmp(key, env->key, key_len) == 0 && key_len == (int)ft_strlen(env->key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
