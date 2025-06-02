#include "minishell.h"

int	lexer_input(t_token **token, char *input)
{
    int		i;
	int 	len;
    int		start;
	char	*part;
	
    i = 0;
	printf(BOLDGREEN "(LEXER_FUNCTION) "RESET GREEN"   input[%c]\n"RESET, input[i]);
    while (input[i])
    {
        if (input[i] && f_isspace(input[i]))
            i++;
		// if (input[i] && (is_quote(input[i]) || is_word_start(input[i])))
		// {
			else if (input[i] && is_quote(input[i]))
			{
				len = inside_quote(input, i, &part);
				if (len == -1 || !part)
					return (0);
				add_token(token, part, WORD);
				i = len;
			}
			else if (input[i] && is_word_start(input[i]))
			{
				start = i;
				while (input[i] && is_word_start(input[i]))
					i++;
				part = f_substring(input, start, i - start);
				if (!part)
					return (0);
				add_token(token, part, WORD);
			}
		// }
		else if (input[i] && is_operator(input[i]))
			i = check_operator(input, i, token);
    }
	print_tokens(*token);
	return (1);
}

int parsing_function(t_token  **token, char *input)										// not finished
{
	if (!check_quote(input))
	{
		printf("quote_error\n");
		return (0);
	}
	if (!lexer_input(token, input)) 
	{
		printf("lexer_error\n");
		return (0);
	}
	return (1);
}

// void    ll(void)
// {
//     system("leaks -q minishell");
// }

int main(int argc, char **argv, char **env)   									// implement in args for third param (env)
{
	char    *input;
	t_token *token;

	(void)argc;
	(void)argv;
	(void)env;
	// atexit(ll);
	while (1)
	{
		input = readline(BOLDRED"[minishell]$> "RESET);
		if (input)
		{
			token = NULL;
			add_history(input);
			printf(BOLDCYAN"INPUT BY READLINE [%s]\n"RESET, input);
			if (parsing_function(&token, input) == 0)
			{
				free_tokens(token);     										// function to free the linked list from tokens
				write_error(1);         										// function to return error depends on num
			}
			free_tokens(token);
			free(input);
		}
		else
			exit(1);
	}
}
