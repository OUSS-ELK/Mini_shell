/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:01 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/11 13:48:18 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/*
 * Returns 1 if str is a valid -n flag (e.g., "-n", "-nnn"), 0 otherwise.
 */
static int	is_valid_n_flag(char *str)
{
	int	i = 2;

	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Joins all args from `args[i]` to end into a single space-separated string.
 */
static char	*join_echo_args(char **args, int i)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	while (args[i])
	{
		tmp = ft_strjoin(result, args[i]);
		free(result);
		result = tmp;
		if (args[i + 1])
		{
			tmp = ft_strjoin(result, " ");
			free(result);
			result = tmp;
		}
		i++;
	}
	return (result);
}

/*
 * Handles printing the echo result with or without trailing newline.
 */
static void	print_echo_output(char *msg, int suppress_newline)
{
	if (suppress_newline)
		ft_putstr_fd(msg, 1);
	else
		ft_putendl_fd(msg, 1);
}

/*
 * ECHO built-in: handles multiple `-n`, space-joined args, and prints to stdout.
 * 
 * @param cmd: A pointer to t_cmd, where `args` contains parsed command arguments
 * @return: Always 0 (success), sets g_exit_status
 */
int	ft_echo(t_cmd *cmd)
{
	int		i;
	int		n_flag;
	char	*output;

	i = 1;
	n_flag = 0;
	output = NULL;

	while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		if (!is_valid_n_flag(cmd->args[i]))
			break ;
		n_flag = 1;
		i++;
	}
	output = join_echo_args(cmd->args, i);
	if (!output)
		return (g_exit_status = 1, 1);
	print_echo_output(output, n_flag);
	free(output);
	return (g_exit_status = 0, 0);
}

// #include "builtins.h"

// /*
// 	-n is valid
// 	-nnn is valid
// 	-n -n is also valid
// */

// static int	check_flag(char *s)
// {
// 	int	i;

// 	if (!s || s[0] != '-')
// 		return (0);
// 	i = 1;
// 	if (s[i] != 'n')
// 		return (0);
// 	while (s[i])
// 	{
// 		if (s[i] != 'n')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// static int	is_writable(t_exec *exec)
// {
// 	if (write(1, NULL, 0) < 0)
// 	{
// 		exec->exit_status = 1;
// 		// print error (minishell echo write error)
// 		return (0);
// 	}
// 	return (1);
// }

// void	echo(char **av, t_exec *exec)
// {
// 	int	print_newline;
// 	int	i;

// 	i = 1;
// 	print_newline = 1;
// 	if (!is_writable(exec))
// 		return ;
// 	while (av[i] && check_flag(av[i]))
// 	{
// 		print_newline = 0;
// 		i++;
// 	}
// 	while (av[i])
// 	{
// 		ft_putstr_fd(av[i], 1);
// 		if (av[i + 1])
// 			ft_putchar_fd(' ', 1);
// 		i++;
// 	}
// 	if (print_newline)
// 		ft_putchar_fd('\n', 1);
// 	exec->exit_status = 0;
// }

