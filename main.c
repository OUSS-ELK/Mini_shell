#include "minishell.h"

int	lexer_input(t_token **token, char *input)
{
    int		i;
	int 	len;
    int		start;
	// char	*join;
	char	*part;
	
    i = 0;
	// test cases :
	// echo "'Hello'" => [Hello]  
	printf(GREEN "(inside lexer) "RESET "input = [%c]\n", input[i]);
    while (input[i])
    {
		printf(" (while) \n");
		// join = NULL;
        while (f_isspace(input[i]))
            i++;
        // should handle mixed quote
		while (input[i] && is_quote(input[i]))											// handle quotes errors
		{
			printf(" when found quote [i = %d]\n", i);
			len = inside_quote(input, i, &part);
			// printf(" after quote [i = %d]\n", i);
			if (len == -1 || !part)
				return (0);
			add_token(token, part, WORD);
			i += len;
			// join = f_strjoin(join, part);
		}
		if (input[i] && is_word_start(input[i]))
		{
			printf(" word start = input[%c]\n", input[i]);
			start = i;
			while (input[i] && is_word_start(input[i]))
				i++;
			part = f_substring(input, start, i - start);
			if (!part)
				return (0);
			add_token(token, part, WORD);
			// join = f_strjoin(join, part);
			// i += f_strlen(join);
		}
		i++;
		// if (join)
		// {
		// 	printf("join\n");
		// 	add_token(token, join, WORD);
		// 	// i += f_strlen(join);
		// }
		// if (is_operator(input[i]))
		// 	i += check_operator(input, i, token);
		// else
		// 	i = find_word(input, i, token);
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
		free(input);
		return (0);
	}
	return (1);
}

// void    ll(void)
// {
//     system("leaks -q minishell");
// }

int main(int argc, char **argv, char **env)   								// implement in args for third param (env)
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
			if (input[0])
			{
				add_history(input);
				write_history("history");
			}
			printf("input reading by readline [%s]\n", input);
			if (parsing_function(&token, input) == 0)
			{
				// free_tokens(token);     									// function to free the linked list from tokens
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
