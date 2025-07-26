/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:53:54 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 09:00:05 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_digit(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	exit_free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*next;

	if (!env || !*env)
		return ;
	tmp = *env;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next;
	}
	*env = NULL;
}

static void	print_and_exit(t_env **env)
{
	g_exit_status = 255;
	write(2, "exit: numeric argument required\n", 32);
	exit_free_env(env);
	exit(g_exit_status);
}

static void	handle_exit_with_arg(t_cmd *cmd, t_env **env)
{
	int	status;

	if (!is_digit(cmd->args[1]))
		print_and_exit(env);
	status = atoi(cmd->args[1]);
	if (status < 0)
		status = 256 + status;
	if (status > 255)
		status = status % 256;
	g_exit_status = status;
	exit_free_env(env);
	exit(g_exit_status);
}

int	ft_exit(t_cmd *cmd, t_env **env)
{
	int	args_count;

	args_count = 0;
	while (cmd->args[args_count])
		args_count++;
	write(1, "exit\n", 5);
	if (args_count > 2)
	{
		g_exit_status = 1;
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	else if (args_count == 2)
		handle_exit_with_arg(cmd, env);
	else
	{
		exit_free_env(env);
		exit(g_exit_status);
	}
	return (0);
}
