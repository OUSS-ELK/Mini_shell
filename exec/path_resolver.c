/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 10:26:01 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/20 10:27:00 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path_value_from_env(t_env *env)
{
	while (env)
	{
		if (!ft_strncmp(env->key, "PATH", 4) && env->key[4] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	search_command_in_path_dirs(char **full_path, char **path_dirs,
		char *cmd)
{
	char	*tmp;

	*full_path = NULL;
	while (*path_dirs)
	{
		tmp = ft_strjoin(*path_dirs, "/");
		if (!tmp)
			return ;
		*full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!*full_path)
			return ;
		if (access(*full_path, X_OK) == 0)
			return ;
		free(*full_path);
		*full_path = NULL;
		path_dirs++;
	}
}

char	*search_command_in_cwd(char *cmd)
{
	char	*cwd;
	char	*tmp;
	char	*result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	tmp = ft_strjoin(cwd, "/");
	free(cwd);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!result)
		return (NULL);
	if (access(result, X_OK) == 0)
		return (result);
	free(result);
	return (NULL);
}

static char	*search_in_path(char *cmd, t_env *env, int *no_file)
{
	char	*full_path;
	char	*path_value;
	char	**path_dirs;

	path_value = get_path_value_from_env(env);
	if (!path_value)
	{
		*no_file = 1;
		return (NULL);
	}
	path_dirs = split_t(path_value, ':');
	if (!path_dirs)
		return (NULL);
	search_command_in_path_dirs(&full_path, path_dirs, cmd);
	ft_free_array(path_dirs);
	return (full_path);
}

char	*resolve_path(char *cmd, t_env *env, int *no_file)
{
	char	*full_path;
	char	*cwd_exec;

	*no_file = 0;
	full_path = check_if_direct_path_valid(cmd);
	if (full_path)
		return (full_path);
	full_path = search_in_path(cmd, env, no_file);
	if (full_path)
		return (full_path);
	cwd_exec = search_command_in_cwd(cmd);
	if (cwd_exec)
		return (cwd_exec);
	return (NULL);
}
