#include "minishell.h"

int f_strlen(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int f_isspace(char *s)
{
	while (*s)
	{
		if (*s == ' ' || (*s >= 9 && *s <= 13))
			return (0);
		s++;
	}
	return (1);
}

char	*filtred_input(char *input)
{
	char	*new_input;

	
	return (new_input);
}

int lexer_function(t_token  **token, char *input)
{
	char    *filtred;
	int     count;

	filtred = NULL;
	if (!token || !input || !*input)
		return (0);
	// cat abc
	filtred = filtring_input(input);   // to check in input for error
	if (!filtred)
		return (0);
	count = count_word(input);         // count word in input
	add_token(*token, count);          // add Node to struct
	return (1);
}


// void    ll(void)
// {
//     system("leaks -q minishell");
// }

int main(void)
{
	char    *input;
	t_token *token;

	// atexit(ll);
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
			if (f_strlen(input) > 0)
			{
				add_history(input);
				write_history("history");
				// printf("add_history\n");
			}
			printf("input reading by readline [%s]\n", input);
			if (lexer_function(&token, input) == 1)
			{
				write_error(1);         // function to return error depends on num
				free_tokens(token);     // function to free the linked list from tokens
			}
			free(input);
		}
		else
			exit(1);
	}
}
