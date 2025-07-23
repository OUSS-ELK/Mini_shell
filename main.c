/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oussama-elk <oussama-elk@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 05:10:15 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 12:00:35 by oussama-elk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

// void	print_open_fds(void)
// {
// 	DIR				*dir;
// 	struct dirent	*ent;
// 	int				count = 0;

// 	dir = opendir("/dev/fd");
// 	if (!dir)
// 	{
// 		perror("opendir");
// 		return ;
// 	}
// 	while ((ent = readdir(dir)))
// 	{
// 		count++;
// 	}
// 	closedir(dir);
// 	printf("🔍 Open FDs: %d\n", count - 2); // exclude . and ..
// }

static void	reset_std_fd(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
}

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

static bool	process_input(char *input, t_env **envr)
{
	t_token	*token;
	t_cmd	*cmd;
	t_exec	exec;

	token = NULL;
	cmd = NULL;
	if (!parsing_function(&token, input, &cmd, envr))
		return (cleanup(token, cmd, input), false);
	exec.env_lst = *envr;
	exec.is_pipe = (cmd && cmd->next);
	if (!check_heredocs(cmd, *envr))
		return (cleanup(token, cmd, input), false);
	if (builtin_check_execute(cmd, &exec, envr) == 1)
		return (cleanup(token, cmd, input), false);
	// printf("before exec\n");
	// print_open_fds();
	if (!execution_main(&exec, cmd, *envr))
		write_error(8);
	// printf("after exec\n");
	// print_open_fds();
	cleanup(token, cmd, input);
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	t_env	*envr;
	char	*input;
	int		stdin_fd;
	int		stdout_fd;

	(void)argc;
	(void)argv;
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
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
			continue ;
		process_input(input, &envr);
		reset_std_fd(stdin_fd, stdout_fd);
	}
	free_env(envr);
	return (0);
}

// int main(int argc, char **argv, char **env)
// {
// 	t_token *token;
// 	char	*input;
// 	t_cmd	*cmd;
// 	t_exec	exec;
// 	t_env	*envr;
// 	int		original_stdin = dup(STDIN_FILENO);
// 	int		original_stdout = dup(STDOUT_FILENO);

// 	(void)argc;
// 	(void)argv;

// 	envr = collect_env(env);  // ✅ collect only once at startup
// 	if (!envr)
// 	{
// 		write(2, "minishell: failed to collect env\n", 33);
// 		return (1);
// 	}

// 	handle_signals();

// 	while (1)
// 	{
// 		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
// 		if (!input)
// 		{
// 			// printf("Input error\n");  // Ctrl + D
// 			break ;
// 		}
// 		add_history(input);
// 		if (all_space(input))
// 		{
// 			free(input);
// 			continue ;
// 		}

// 		token = NULL;
// 		cmd = NULL;

// 		if (!parsing_function(&token, input, &cmd, &envr)) 
			// ✅ pass envr directly
// 		{
// 			cleanup(token, cmd, input);  // Don't clean envr here
// 			continue ;
// 		}

// 		exec.env_lst = envr;
// 		exec.is_pipe = (cmd && cmd->next);

// 		printf("DEBUG: parsing_function returned success\n");
// 		print_cmds(cmd);

// 		if (!check_heredocs(cmd, envr))
// 		{
// 			printf("Error heredoc\n");
// 			cleanup(token, cmd, input);
// 			continue ;
// 		}

// 		if (builtin_check_execute(cmd, &exec, &envr) == 1)
// 		{
// 			cleanup(token, cmd, input);
// 			continue ;
// 		}

// 		// printf("DEBUG: Running execution_main\n");

// 		if (!execution_main(&exec, cmd, envr))
// 		{
// 			write_error(8);
// 		}

// 		cleanup(token, cmd, input);  // envr remains persistent

// 		dup2(original_stdin, STDIN_FILENO);
// 		dup2(original_stdout, STDOUT_FILENO);
// 	}
// 	free_env(envr);
// 	return (0);
// }
