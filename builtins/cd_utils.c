/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:50:18 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 08:45:56 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_env_node(t_env **env, const char *key, const char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	tmp = *env;
	if (!tmp)
	{
		*env = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	get_args_count(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

t_env	*get_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*ft_getenv_value(t_env *env, char *key)
{
	t_env	*node;

	node = get_env_node(env, key);
	if (node && node->value)
		return (node->value);
	return (NULL);
}

void	update_pwd_vars(t_env *env, char *old_path)
{
	t_env	*oldpwd;
	t_env	*pwd;
	char	*cwd;

	oldpwd = get_env_node(env, "OLDPWD");
	pwd = get_env_node(env, "PWD");
	cwd = getcwd(NULL, 0);
	if (oldpwd)
	{
		free(oldpwd->value);
		oldpwd->value = ft_strdup(old_path);
	}
	else
		add_env_node(&env, "OLDPWD", old_path);
	if (pwd)
	{
		free(pwd->value);
		pwd->value = cwd;
	}
	else
		add_env_node(&env, "PWD", cwd);
}
