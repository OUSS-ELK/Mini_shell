/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:28:16 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 08:44:26 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	resolve_cd_home_or_oldpwd(char **args, t_env *env, char **target)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		*target = ft_getenv_value(env, "HOME");
		if (!*target)
		{
			write(2, "minishell: cd: HOME not set\n", 29);
			return (1);
		}
		*target = ft_strdup(*target);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		*target = ft_getenv_value(env, "OLDPWD");
		if (!*target)
		{
			write(2, "minishell: cd: OLDPWD not set\n", 31);
			return (1);
		}
		*target = ft_strdup(*target);
	}
	else
		return (-1);
	return (0);
}

int	resolve_cd_target(char **args, t_env *env, char **target, char *old_path)
{
	char	*tmp;
	int		ret;

	ret = resolve_cd_home_or_oldpwd(args, env, target);
	if (ret != -1)
		return (ret);
	tmp = expand_tilde(args[1], env);
	if (!tmp)
	{
		if (old_path)
			free(old_path);
		write(2, "minishell: cd: HOME not set\n", 29);
		return (1);
	}
	*target = tmp;
	return (0);
}

static char	*get_old_path(t_env *env)
{
	char	*old_path;

	old_path = ft_getenv_value(env, "PWD");
	if (old_path)
		return (ft_strdup(old_path));
	return (getcwd(NULL, 0));
}

int	cd_change_dir(char **args, t_env *env, char *old_path)
{
	char	*target;
	int		is_dash;

	if (resolve_cd_target(args, env, &target, old_path))
		return (free(old_path), g_exit_status = 1, 1);
	is_dash = (args[1] && ft_strcmp(args[1], "-") == 0);
	if (chdir(target) != 0)
		return (perror(target), free(target), free(old_path),
			g_exit_status = 1, 1);
	if (is_dash)
		ft_putendl_fd(target, 1);
	update_pwd_vars(env, old_path);
	g_exit_status = 0;
	free(old_path);
	free(target);
	return (0);
}

int	ft_cd(char **args, t_env **env_ptr)
{
	t_env	*env;
	char	*old_path;
	char	*target;

	env = *env_ptr;
	if (get_args_count(args) > 2)
		return (write(2, "minishell: cd: too many arguments\n", 34),
			g_exit_status = 1, 1);
	old_path = get_old_path(env);
	if (!old_path)
	{
		if (!args[1])
		{
			target = ft_strdup("/");
			if (!target)
				return (perror("malloc"), g_exit_status = 1, 1);
			if (chdir(target) != 0)
				return (perror(target), free(target), g_exit_status = 1, 1);
			ft_putendl_fd(target, 1);
			g_exit_status = 0;
			return (free(target), 0);
		}
		return (perror("getcwd"), g_exit_status = 1, 1);
	}
	return (cd_change_dir(args, env, old_path));
}
