#include "minishell.h"

int	lexer_input(t_token **token, char *input)
{
    int		i;
	int 	len;
    int		start;
	char	*join;
	char	*part;
	
    i = 0;
	// test cases :
	// echo "'Hello'" => [Hello]  
	printf(BOLDGREEN "(inside lexer) "RESET GREEN"input = [%c]\n"RESET, input[i]);
    while (input[i])
    {
		join = NULL;
        while (f_isspace(input[i]))
            i++;
        // should handle mixed quote
		while (input[i] && (is_quote(input[i]) || is_word_start(input[i])))						// handle quotes errors
		{
			if (is_quote(input[i]))
			{
				// check for one single quote error
				// printf(" when found quote [i = %d]\n", i);
				len = inside_quote(input, i, &part);
				// printf(" after quote len = %d | [i = %d]\n", len, i);
				if (len == -1 || !part)
				{
					free(join);
					return (0);
				}
				// add_token(token, part, WORD);
				join = f_strjoin(join, part);
				i = len;
			}
			else if (is_word_start(input[i]))
			{
				// printf(" word start = input[%c]\n", input[i]);
				start = i;
				while (is_word_start(input[i]))
					i++;
				// printf("i = %d | start = %d\n", i, start);
				part = f_substring(input, start, i - start);
				if (!part)
					return (0);
				// add_token(token, part, WORD);
				join = f_strjoin(join, part);
				// i += f_strlen(join);
			}
		}
		if (join)
		{
			// printf("join\n");
			add_token(token, join, WORD);
			// i += f_strlen(join);
		}
		if (is_operator(input[i]))
			i = check_operator(input, i, token);
		else if (f_isspace(input[i]))
			i++;
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
