#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_token *token;
	char	*input;
	t_cmd	*cmd;

	(void)argc;
	(void)argv;

	atexit(ll);
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (!input)																	// Ctrl + D (EOF) 
			break ;
		if (all_space(input))														// Empty or spaces
		{
			free(input);
			continue ;
		}
		if (input)
		{
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
		}
		cleanup(token, cmd, input);
	}
	return (0);
}
