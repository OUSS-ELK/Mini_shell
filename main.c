#include "minishell.h"

int	lexer_input(t_token **token, char *input)
{
    int		i;
	int 	len;
    int		start;
	char	*part;
	
    i = 0;  
	// printf(BOLDGREEN "(inside lexer) "RESET GREEN"input = [%c]\n"RESET, input[i]);
    while (input[i])
    {
        while (input[i] && f_isspace(input[i]))
            i++;
		while (input[i] && (is_quote(input[i]) || is_word_start(input[i])))
		{
			// printf(BLUE"inside (while) quote or word   input[%c] \n"RESET, input[i]);
			if (input[i] && is_quote(input[i]))
			{
			// printf(BLUE"is_quotes input[%c] \n"RESET, input[i]);
				len = inside_quote(input, i, &part);
				// printf(YELLOW"after quote len = %d\n"RESET, len);
				if (len == -1 || !part)
					return (0);
				add_token(token, part, WORD);
				printf("SPACE  INPUT[%c] | TOKEN->value\n", input[len]);
				print_tokens(*token);
				if (input[len] && f_isspace(input[len]))
				{
					(*token)->space = 1;
					while (input[len] && f_isspace(input[len]))
						len++;
				}
				else
					(*token)->space = 0;
				i = len;
			}
			else if (input[i] && is_word_start(input[i]))
			{
			// printf(BLUE"is_word input[%c] \n"RESET, input[i]);
				start = i;
				while (input[i] && is_word_start(input[i]))
					i++;
				part = f_substring(input, start, i - start);
				if (!part)
					return (0);
				add_token(token, part, WORD);
				len = i;
				printf("2 SPACE  INPUT[%c]\n", input[len]);
				if (input[len] && f_isspace(input[len]))
				{
					(*token)->space = 1;
					while (input[len] && f_isspace(input[len]))
						len++;
				}
				else
					(*token)->space = 0;
				i = len;
			}
		}
		if (input[i] && is_operator(input[i]))
			i = check_operator(input, i, token);
		// else if (input[i] && f_isspace(input[i]))
		// {
		// 	// printf("SPACE  INPUT[%c]\n", input[i]);
		// 	i++;
		// }
		// else if (input[i] && is_word_start(input[i]))
		// {
		// 	printf("WORD  INPUT[%c]\n", input[i]);
		// 	(*token)->space = 0;
		// 	i++;
		// }
		// else
		// 	i++;
    }
	printf("last tokens\n");
	print_tokens(*token);
	return (1);
}

t_token	*new_token(t_token **token)
{
	char	*n_content;
	t_token	*current;
	t_token	*next;

	current = *token;
	while (current && current->next)
	{
		if (current->space == 1)
		{
			next = current->next;
			n_content = f_strjoin(current->token, next->token);
			printf(GREEN"new_token = %s\n"RESET, n_content);
			if (!n_content)
				return (0);
			free(current->token);		// free old content
			current->token = n_content;	// remove next token from the list
			free(next->token);			// free next token's content
			free(next);					// free next token
		}
		else
			current = current->next;
	}
	print_tokens(*token);
	return (*token);
}

int parsing_function(t_token  **token, char *input)						// not finished
{
	if (!token || !input || !*input)
		return (0);

	if (!lexer_input(token, input))            							// check input for error 
	{
		printf("lexer_error\n");
		return (0);
	}
	// printf("after lexing\n");
	// // should joined mixed words that space flaged with 0
	// token = new_token(token);
	return (1);
}

// void    ll(void)
// {
//     system("leaks -q minishell");
// }

int main(int argc, char **argv, char **env)   							// implement in args for third param (env)
{
	char    *input;
	t_token *token;

	(void)argc;
	(void)argv;
	(void)env;
	// atexit(ll);
	token = NULL;
	while (1)
	{
		input = readline(BOLDRED"[minishell]$> "RESET);
		if (input)
		{
			if (strncmp(input, "exit", 4) == 0)
			{
				free(input);
				return (1);
			}
			// if (input[0])
			// {
			// 	add_history(input);
			// 	write_history("history");
			// }
			printf(BOLDCYAN"INPUT READ [%s]\n"RESET, input);
			if (parsing_function(&token, input) == 0)
			{
				free_tokens(token);     									// function to free the linked list from tokens
				write_error(1);         									// function to return error depends on num
			}
			// free_tokens(token);
			free(input);
			token = NULL;
		}
		else
			exit(1);
	}
}
