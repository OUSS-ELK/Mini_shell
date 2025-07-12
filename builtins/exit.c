#include "minishell.h"

#define EXIT_ERROR "exit: numeric argument required\n"

/*
 * Checks if a string is a valid numeric string.
 * Accepts optional leading + or -.
 */
int	is_digit(char *str)
{
	int	i = 0;

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

/*
 * Frees the environment list.
 */
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

/*
 * Prints error message and exits with status 255.
 */
static void	print_and_exit(t_env **env)
{
	g_exit_status = 255;
	write(2, EXIT_ERROR, 32);
	exit_free_env(env);
	exit(g_exit_status);
}

/*
 * Exit builtin: exits the shell with provided status (or g_exit_status).
 * Handles too many arguments, non-numeric input, and proper cleanup.
 */
int	ft_exit(t_cmd *cmd, t_env **env)
{
	int	status;
	int	args_count = 0;

	// Count args (args[0] is "exit")
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
	{
		if (!is_digit(cmd->args[1]))
			print_and_exit(env);
		status = atoi(cmd->args[1]);
		if (status < 0)
			status = 256 + status;
		if (status > 255)
			status = status % 256;
		g_exit_status = status;
	}
	else
		g_exit_status = 0;
	exit_free_env(env);
	exit(g_exit_status);
}
