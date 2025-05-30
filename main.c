#include "minishell.h"

t_token	*last_token(t_token	**token)
{
	t_token	*last;

	last = *token;
	while (last != NULL)
		last = last->next;
	return (last);
}

int	lexer_input(t_token **token, char *input)
{
    int		i;
	int 	len;
    int		start;
	// char	*join;
	char	*part;
	t_token	*tmp;
	
    i = 0;  
	printf(BOLDGREEN "(inside lexer) "RESET GREEN"input = [%c]\n"RESET, input[i]);
    while (input[i])
    {
		// join = NULL;
        while (input[i] && f_isspace(input[i]))
            i++;
        // should handle mixed quote
		while (input[i] && (is_quote(input[i]) || is_word_start(input[i])))									// handle quotes errors
		{
			if (input[i] && is_quote(input[i]))
			{
				len = inside_quote(input, i, &part);
				if (len == -1 || !part)
					return (0);
				add_token(token, part, WORD);
				if (input[i] && input[i] == ' ')
				{
			
					tmp = last_token(token);
					tmp->space = 1;				
				}
				else
				{
					tmp = last_token(token);
					tmp->space = 0;				
				}
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
				if (input[i] && input[i] == ' ')
				{
			
					tmp = last_token(token);
					tmp->space = 1;				
				}
				else
				{
					tmp = last_token(token);
					tmp->space = 0;				
				}
			}

		}
		if (input[i] && is_operator(input[i]))
			i = check_operator(input, i, token);
		else if (input[i] && f_isspace(input[i]))
			i++;
		// printf("error input[%c]\n", input[i]);
    }

	print_tokens(*token);
	return (1);
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
	// should check for space if 0 need to join string
	// check after if there is space and flag it in struct space
	
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
		input = readline("[minishell]$> ");
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
