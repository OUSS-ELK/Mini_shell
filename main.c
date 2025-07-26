/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 05:10:15 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 07:39:15 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

static bool	should_skip_input(char *input)
{
	if (!input)
		return (true);
	add_history(input);
	if (all_space(input))
	{
		free(input);
		return (true);
	}
	return (false);
}

static void	reset_std_fd(int stdin_fd, int stdout_fd)
{
	if (stdin_fd != -1)
	{
		dup2(stdin_fd, STDIN_FILENO);
		if (stdin_fd != STDIN_FILENO)
			close(stdin_fd);
	}
	if (stdout_fd != -1)
	{
		dup2(stdout_fd, STDOUT_FILENO);
		if (stdout_fd != STDOUT_FILENO)
			close(stdout_fd);
	}
}

static bool	process_input(char *input, t_env **envr)
{
	t_token	*token = NULL;
	t_cmd	*cmd = NULL;
	t_exec	exec;
	int		stdin_fd;
	int		stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	if (stdin_fd == -1)
	{
		perror("dup STDIN_FILENO");
		return (false);
	}
	stdout_fd = dup(STDOUT_FILENO);
	if (stdout_fd == -1)
	{
		perror("dup STDOUT_FILENO");
		close(stdin_fd);
		return (false);
	}

	if (!parsing_function(&token, input, &cmd, envr))
	{
		reset_std_fd(stdin_fd, stdout_fd);
		return (cleanup(token, cmd, input), false);
	}
	exec.env_lst = *envr;
	exec.is_pipe = (cmd && cmd->next);
	if (!check_heredocs(cmd, *envr))
	{
		reset_std_fd(stdin_fd, stdout_fd);
		return (cleanup(token, cmd, input), false);
	}
	if (builtin_check_execute(cmd, &exec, envr) == 1)
	{
		reset_std_fd(stdin_fd, stdout_fd);
		return (cleanup(token, cmd, input), false);
	}
	if (!execution_main(&exec, cmd, *envr))
		write_error(8);
	reset_std_fd(stdin_fd, stdout_fd);
	cleanup(token, cmd, input);
	return (true);
}

// static void	reset_std_fd(int stdin_fd, int stdout_fd)
// {
// 	dup2(stdin_fd, STDIN_FILENO);
// 	dup2(stdout_fd, STDOUT_FILENO);
// 	if (stdin_fd != STDIN_FILENO)
// 		close(stdin_fd);
// 	if (stdout_fd != STDOUT_FILENO)
// 		close(stdout_fd);
// }


// static bool	process_input(char *input, t_env **envr)
// {
// 	t_token	*token;
// 	t_cmd	*cmd;
// 	t_exec	exec;
// 	int		stdin_fd;
// 	int		stdout_fd;

// 	token = NULL;
// 	cmd = NULL;
// 	stdin_fd = dup(STDIN_FILENO);
// 	stdout_fd = dup(STDOUT_FILENO);
// 	if (!parsing_function(&token, input, &cmd, envr))
// 		return (cleanup(token, cmd, input), false);
// 	exec.env_lst = *envr;
// 	exec.is_pipe = (cmd && cmd->next);
// 	if (!check_heredocs(cmd, *envr))
// 		return (cleanup(token, cmd, input), false);
// 	if (builtin_check_execute(cmd, &exec, envr) == 1)
// 		return (cleanup(token, cmd, input), reset_std_fd(stdin_fd, stdout_fd),
// 			false);
// 	if (!execution_main(&exec, cmd, *envr))
// 		write_error(8);
// 	reset_std_fd(stdin_fd, stdout_fd);
// 	cleanup(token, cmd, input);
// 	return (true);
// }

int	main(int argc, char **argv, char **env)
{
	t_env	*envr;
	char	*input;

	(void)argc;
	(void)argv;
	envr = collect_env(env);
	if (!envr)
		return (write(2, "minishell: failed to collect env\n", 33), 1);
	handle_signals();
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (!input)
			break ;
		if (should_skip_input(input))
		{
			free(input);
			continue ;
		}
		process_input(input, &envr);
	}
	free_env(envr);
	return (g_exit_status);
}
