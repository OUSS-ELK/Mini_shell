#include "minishell.h"

int	parsing_function(t_token **token, char *input, char **env, t_cmd **cmd)
{
	t_env	*envr;

	envr = collect_env(env);
	if (!envr)
		return (0);
	if (!check_quote(input))
	{
		write_error(2);
		return (free_env(envr), 0);
	}
	if (!lexer_input(token, input, envr))
	{
		write_error(3);
		return (free_env(envr), 0);
	}
	*cmd = parse_cmd(token);
	if (!*cmd)
		return (free_env(envr), 0);
	free_env(envr);
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_token	*token;
	char	*input;
	t_cmd	*cmd;

	(void)argc;
	(void)argv;
	// atexit(ll);
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (!input)
			break ;
		if (all_space(input))
		{
			free(input);
			continue ;
		}
		token = NULL;
		cmd = NULL;
		add_history(input);
		printf(BOLDCYAN "INPUT BY READLINE [%s]\n" RESET, input);
		if (!parsing_function(&token, input, env, &cmd))
		{
			write_error(1);
			cleanup(token, cmd, input);
			continue ;
		}
		print_cmds(cmd);
		cleanup(token, cmd, input);
	}
	return (0);
}
