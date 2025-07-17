/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:28:16 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/17 08:48:17 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_env_node(t_env **env, const char *key, const char *value)
{
	t_env *new = malloc(sizeof(t_env));
	if (!new)
		return;
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;

	// Append at the end
	t_env *tmp = *env;
	if (!tmp)
	{
		*env = new;
		return;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/* Helper: Get size of double pointer array */
int	get_args_count(char **args)
{
	int	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

/* Helper: Search environment linked list for a key */
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

/* Helper: Get value of env var by key */
char	*ft_getenv_value(t_env *env, char *key)
{
	t_env *node = get_env_node(env, key);
	return (node && node->value) ? node->value : NULL;
}

/* Updates PWD and OLDPWD after successful cd */
void	update_pwd_vars(t_env *env, char *new_path, char *old_path)
{
	t_env	*oldpwd = get_env_node(env, "OLDPWD");
	t_env	*pwd = get_env_node(env, "PWD");
	char	*cwd = getcwd(NULL, 0);

	if (oldpwd)
	{
		free(oldpwd->value);
		oldpwd->value = ft_strdup(old_path);
	}
	else
		add_env_node(&env, "OLDPWD", old_path);  // ✅ Add it if missing

	if (pwd)
	{
		free(pwd->value);
		pwd->value = cwd;
	}
	else
		add_env_node(&env, "PWD", cwd);  // ✅ Add it if missing
}



/* Expands `~` into $HOME if present */
char	*expand_tilde(char *input, t_env *env)
{
	char *home;
	char *expanded;

	if (!input || input[0] != '~')
		return (ft_strdup(input));
	home = ft_getenv_value(env, "HOME");
	if (!home)
		return (NULL);
	expanded = ft_strjoin(home, input + 1);
	return (expanded);
}

/* Determines the target path based on args (HOME, ~, OLDPWD, etc.) */
int	resolve_cd_target(char **args, t_env *env, char **target, char *old_path)
{
	char *tmp;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		*target = ft_getenv_value(env, "HOME");
		if (!*target)
			return (free(old_path), write(2, "minishell: cd: HOME not set\n", 29), 1);
		*target = ft_strdup(*target);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		*target = ft_getenv_value(env, "OLDPWD");
		if (!*target)
			return (free(old_path), write(2, "minishell: cd: OLDPWD not set\n", 31), 1);
		*target = ft_strdup(*target);
	}
	else
	{
		tmp = expand_tilde(args[1], env);
		if (!tmp)
			return (free(old_path), write(2, "minishell: cd: HOME not set\n", 29), 1);
		*target = tmp;
	}
	return (0);
}

/* Main cd builtin */
int	ft_cd(char **args, t_env **env_ptr)
{
	t_env	*env = *env_ptr;
	char	*target;
	char	*old_path;
	int		is_dash;

	if (get_args_count(args) > 2)
		return (write(2, "minishell: cd: too many arguments\n", 34), g_exit_status = 1, 1);

	old_path = ft_getenv_value(env, "PWD");
	old_path = old_path ? ft_strdup(old_path) : getcwd(NULL, 0);
	if (!old_path)
		return (perror("getcwd"), g_exit_status = 1, 1);

	if (resolve_cd_target(args, env, &target, old_path))
		return (g_exit_status = 1, 1);

	is_dash = (args[1] && ft_strcmp(args[1], "-") == 0);
	if (chdir(target) != 0)
	{
		perror(target);
		free(target);
		free(old_path);
		g_exit_status = 1;
		return (1);
	}

	if (is_dash)
	{
		write(1, target, ft_strlen(target));
		write(1, "\n", 1);
	}

	update_pwd_vars(env, target, old_path);
	g_exit_status = 0;
	return (0);
}


// static void	update_pwd_vars(t_env **env_lst, t_exec *exec)
// {
// 	char	*old_pwd;
// 	char	*new_pwd;
// 	char	*av[3];

// 	av[2] = NULL;
// 	old_pwd = getcwd(NULL, 0);
// 	if (old_pwd)
// 	{
// 		av[1] = ft_strjoin("OLDPWD=", old_pwd);
// 		builtin_export(av, env_lst, exec);
// 		free(old_pwd);
// 	}
// 	new_pwd = getcwd(NULL, 0);
// 	if (new_pwd)
// 	{
// 		av[1] = ft_strjoin("PWD=", new_pwd);
// 		builtin_export(av, env_lst, exec);
// 		free(new_pwd);
// 	}
// }

// // change directory to $HOME
// static int	go_home(t_env *env_lst)
// {
// 	t_env	*home;

// 	while (home && ft_strcmp(home->key, "HOME") != 0) // look for HOME in env_lst
// 		home = home->next;
// 	if (!home || !home->value) // if not found or has no value
// 	{
// 		// print error (minishell cd HOME not set)
// 		return (0);
// 	}
// 	if (chdir(home->value) != 0)
// 	{
// 		// print error (minishell cd home->value)
// 		return (0);
// 	}
// 	return (1);
// }

// // main function
// void	ft_cd(char **args, t_env **env_lst, t_exec *exec)
// {
// 	if (!args || !args[0] || !env_lst || !(*env_lst))
// 		return ;
// 	if (!args[1]) // cd with no ars == go to $HOME
// 	{
// 		if (!go_home(*env_lst))
// 		{
// 			exec->exit_status = 1;
// 			return ;
// 		}
// 	}
// 	else if (chdir(args[1]) != 0) // fail
// 	{
// 		// print error (minishell, cd, args[1])
// 		exec->exit_status = 1;
// 		return ;
// 	}
// 	update_pwd_vars(env_lst, exec);
// 	exec->exit_status = 0;
// }