#include "minishell.h"

int	g_exit_status = 0;

int parsing_function(t_token **token, char *input, t_cmd **cmd, t_env **envr)
{
	if (!check_quote(input))
	{
		write_error(2);
		return (0);
	}
	if (!lexer_input(token, input, *envr))
	{
		write_error(3);
		return (0);
	}
	*cmd = parse_cmd(token);
	if (!*cmd)
		return (0);
	return (1);
}

int main(int argc, char **argv, char **env)
{
	t_token *token;
	char	*input;
	t_cmd	*cmd;
	t_exec	exec;
	t_env	*envr;
	int		original_stdin = dup(STDIN_FILENO);
	int		original_stdout = dup(STDOUT_FILENO);

	(void)argc;
	(void)argv;

	envr = collect_env(env);  // ✅ collect only once at startup
	if (!envr)
	{
		write(2, "minishell: failed to collect env\n", 33);
		return (1);
	}

	handle_signals();

	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (!input)
		{
			// printf("Input error\n");  // Ctrl + D
			break;
		}
		add_history(input);
		if (all_space(input))
		{
			free(input);
			continue;
		}

		token = NULL;
		cmd = NULL;

		if (!parsing_function(&token, input, &cmd, &envr))  // ✅ pass envr directly
		{
			write_error(1);
			cleanup(token, cmd, input, NULL);  // Don't clean envr here
			continue;
		}

		exec.env_lst = envr;
		exec.is_pipe = (cmd && cmd->next);

		// printf("DEBUG: parsing_function returned success\n");
		print_cmds(cmd);

		if (!check_heredocs(cmd, envr))
		{
			printf("Error heredoc\n");
			cleanup(token, cmd, input, NULL);
			continue;
		}

		if (builtin_check_execute(cmd, &exec, &envr) == 1)
		{
			cleanup(token, cmd, input, NULL);
			continue;
		}

		// printf("DEBUG: Running execution_main\n");

		if (!execution_main(&exec, cmd, envr))
		{
			write_error(8);
		}

		cleanup(token, cmd, input, NULL);  // envr remains persistent

		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
	}
	return (0);
}
