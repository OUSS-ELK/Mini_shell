/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 05:10:15 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 08:58:47 by bel-abde         ###   ########.fr       */
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
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

static bool	process_input(char *input, t_env **envr)
{
	t_token	*token;
	t_cmd	*cmd;
	t_exec	exec;
	int		stdin_fd;
	int		stdout_fd;

	token = NULL;
	cmd = NULL;
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!parsing_function(&token, input, &cmd, envr))
		return (cleanup(token, cmd, input), false);
	exec.env_lst = *envr;
	exec.is_pipe = (cmd && cmd->next);
	if (!check_heredocs(cmd, *envr))
		return (cleanup(token, cmd, input), false);
	if (builtin_check_execute(cmd, &exec, envr) == 1)
		return (cleanup(token, cmd, input), reset_std_fd(stdin_fd, stdout_fd),
			false);
	if (!execution_main(&exec, cmd, *envr))
		write_error(8);
	reset_std_fd(stdin_fd, stdout_fd);
	cleanup(token, cmd, input);
	return (true);
}

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
