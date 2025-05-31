#include "minishell.h"

int	lexer_input(t_token **token, char *input)
{
    int		i;
	int 	len;
    int		start;
	char	*part;
	
    i = 0;
	printf(BOLDGREEN "(inside lexer) "RESET GREEN"input = [%c]\n"RESET, input[i]);
    while (input[i])
    {
        while (input[i] && f_isspace(input[i]))
            i++;
		while (input[i] && (is_quote(input[i]) || is_word_start(input[i])))
		{
			printf(BLUE"inside (while) quote or word   input[%c] \n"RESET, input[i]);
			if (input[i] && is_quote(input[i]))
			{
				printf(BLUE"is_quotes input[%c] \n"RESET, input[i]);
				len = inside_quote(input, i, &part);
				printf(YELLOW"after quote len = %d\n"RESET, len);
				if (len == -1 || !part)
				{
					free(part);
					return (0);
				}
				// add_token(token, part, WORD);
				// printf("list token in quotes\n");
				// print_tokens(*token);
				// if (input[len] && f_isspace(input[len]))
				// {
				// 	(*token)->space = 1;
				// 	// while (input[len] && f_isspace(input[len]))
				// 	// 	len++;
				// }
				// else
				// 	(*token)->space = 0;
				i = len;
			}
			else if (input[i] && is_word_start(input[i]))
			{
				printf(BLUE"is_word input[%c] \n"RESET, input[i]);
				start = i;
				while (input[i] && is_word_start(input[i]))
					i++;
				part = f_substring(input, start, i - start);
				if (!part)
					return (0);
				// add_token(token, part, WORD);
				// len = i;
				// printf(" I - i[%d]\n", i);
				// if (input[len] && f_isspace(input[len]))
				// {
				// 	// printf("space in token %d\n", (*token)->space);
				// 	(*token)->space = 1;
				// 	// while (input[len] && f_isspace(input[len]))
				// 	// 	len++;
				// }
				// else
				// 	(*token)->space = 0;
				// // i = len;
				// printf(" II - i[%d]\n", i);
			}
		}
		if (input[i] && is_operator(input[i]))
			i += check_operator(input, i, token);
		// else if (input[i] && f_isspace(input[i]))
		// {
		// 	printf("SPACE  INPUT[%c]\n", input[i]);
		// 	i++;
		// }
		// else if (input[i] && is_word_start(input[i]))
		// {
		// 	printf("WORD  INPUT[%c]\n", input[i]);
		// 	(*token)->space = 0;
		// 	i++;
		// }
		else
			i++;
    }
	// printf("last tokens\n");
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
	// token = new_token(token);
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
			printf(BOLDCYAN"INPUT READ [%s]\n"RESET, input);
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
