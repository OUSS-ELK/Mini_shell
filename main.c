#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_token *token;
	char	*input;
	t_cmd	*cmd;

	(void)argc;
	(void)argv;

	// atexit(ll);
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (!input)																// Ctrl + D (EOF) 
			break ;
		if (all_space(input))													// Empty or spaces
		{
			free(input);
			continue ;
		}
		if (input)
		{
			token = NULL;
			add_history(input);
			printf(BOLDCYAN "INPUT BY READLINE [%s]\n" RESET, input);
			if (!parsing_function(&token, input, env, &cmd))
			{
				write_error(1);													// function to return error depends on num
				free_tokens(token); 											// function to free the linked list from tokens
				free_cmd(cmd);
				continue ;
			}
			print_cmds(cmd);
		}
		free_tokens(token);
		free_cmd(cmd);
		free(input);
	}
	return (0);
}
