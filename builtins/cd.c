/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:28:16 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/28 12:28:23 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	update_pwd_vars(t_env **env_lst, t_exec *exec)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*av[3];

	av[2] = NULL;
	old_pwd = getcwd(NULL, 0);
	if (old_pwd)
	{
		av[1] = ft_strjoin("OLDPWD=", old_pwd);
		builtin_export(av, env_lst, exec);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		av[1] = ft_strjoin("PWD=", new_pwd);
		builtin_export(av, env_lst, exec);
		free(new_pwd);
	}
}

// change directory to $HOME
static int	go_home(t_env *env_lst)
{
	t_env	*home;

	while (home && ft_strcmp(home->key, "HOME") != 0) // look for HOME in env_lst
		home = home->next;
	if (!home || !home->value) // if not found or has no value
	{
		// print error (minishell cd HOME not set)
		return (0);
	}
	if (chdir(home->value) != 0)
	{
		// print error (minishell cd home->value)
		return (0);
	}
	return (1);
}

// main function
void	builtin_cd(char **args, t_env **env_lst, t_exec *exec)
{
	if (!args || !args[0] || !env_lst || !(*env_lst))
		return ;
	if (!args[1]) // cd with no ars == go to $HOME
	{
		if (!go_home(*env_lst))
		{
			exec->exit_status = 1;
			return ;
		}
	}
	else if (chdir(args[1]) != 0) // fail
	{
		// print error (minishell, cd, args[1])
		exec->exit_status = 1;
		return ;
	}
	update_pwd_vars(env_lst, exec);
	exec->exit_status = 0;
}