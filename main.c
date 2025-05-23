#include "minishell.h"

int	lexer_input(t_token **token, char *input)
{
    int	i;
	
	// skip space
	// checking closed quotes
	// "word  > ("") not closed error.
	// inside quots ("hello world") => one word
    i = 0;
	// printf("(inside lexer) input = [%c]\n", input[i]);
    while (input[i])
    {
        if (f_isspace(input[i]))												// skip white space
            i++;
        else if (is_quote(input[i]))											// handle quotes errors
		{
			printf(" when found quote [i = %d]\n", i);
			i = inside_quote(input, i, token);
			printf(" after quote [i = %d]\n", i);
			if (i == -1)
				return (0);
		}
		else if (is_operator(input[i]))
			i = check_operator(input, i, token);
		else
			i = find_word(input, i, token);
    }
	print_tokens(*token);
	return (1);
}

int parsing_function(t_token  **token, char *input)							// not finished
{
	if (!token || !input || !*input)
		return (0);

	if (!lexer_input(token, input))            								// check input for error 
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

int main(void)   // implement in args for third param (env)
{
	char    *input;
	t_token *token;

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
			if (input[0])
			{
				add_history(input);
				write_history("history");
			}
			printf("input reading by readline [%s]\n", input);
			if (parsing_function(&token, input) == 0)
			{
				free_tokens(token);     						// function to free the linked list from tokens
				write_error(1);         						// function to return error depends on num
			}
			free_tokens(token);
			free(input);
			token = NULL;
		}
		else
			exit(1);
	}
}
