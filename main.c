#include "minishell.h"

int lexer_input(t_token **token, char *input, t_env *env)
{
	int i;
	int len;
	int start;
	char *part;

	i = 0;
	printf(BOLDGREEN "(LEXER_FUNCTION) " RESET GREEN "   input[%c]\n" RESET, input[i]);
	while (input[i])
	{
		if (input[i] && f_isspace(input[i]))
			i++;
		else if (input[i] && is_quote(input[i]))
		{
			len = inside_quote(input, i, &part, token);
			if (len == -1 || !part)
				return (0);
			add_token(token, part, WORD);
			i = len;
		}
		else if (input[i] && is_word_start(input[i]) && input[i] != '$')
		{
			start = i;
			while (input[i] && is_word_start(input[i]))
				i++;
			part = f_substring(input, start, i - start);
			if (!part)
				return (0);
			add_token(token, part, WORD);
		}
		else if (input[i] && is_operator(input[i]))
			i = check_operator(input, i, token);
		else if (valid_expand(input[i], input[i + 1]) == 1)
		{
			printf(BLACK"find $ sign\n"RESET);
			i = expanding_var(token, i, input, env);
			if (i == -1)
				return (0);
		}
	}
	print_tokens(*token);
	return (1);
}

int parsing_function(t_token **token, char *input, char **env)
{
	t_env	*envr;

	envr = collect_env(env); 						// collecte environement variables
	if (!envr)
		return (0);
	if (!check_quote(input))
	{
		write_error(2);
		return (0);
	}
	if (!lexer_input(token, input, envr))
	{
		write_error(3);
		free_env(envr);
		return (0);
	}
	free_env(envr);
	return (1);
}

// void    ll(void)
// {
//     system("leaks -q minishell");
// }

int main(int argc, char **argv, char **env)
{
	t_token *token;
	char	*input;

	(void)argc;
	(void)argv;

	// atexit(ll);
	while (1)
	{
		input = readline(BOLDRED "[MINI_SHELL]$> " RESET);
		if (input)
		{
			token = NULL;
			add_history(input);
			printf(BOLDCYAN "INPUT BY READLINE [%s]\n" RESET, input);
			if (parsing_function(&token, input, env) == 0)
			{
				free_tokens(token); 							// function to free the linked list from tokens
				write_error(1);									// function to return error depends on num
			}
			free_tokens(token);
			free(input);
		}
		else
			exit(1);
	}
	return (0);
}

