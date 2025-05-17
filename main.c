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
    if (s)
    {
	    while (*s)
	    {
		    if (*s == ' ' || (*s >= 9 && *s <= 13))
			    return (0);
		    s++;
	    }
    }
	return (1);
}

char	*filtring_input(char *input)                // not finished
{
	char	*new_input;
    int     i;
    int     len;

    len = f_strlen(input);
    new_input = malloc(len + 1);
    if (!new_input)
        return (NULL);
    strncpy(new_input, input, len + 1);
    i = 0;
    while (new_input[i])
    {
        if (new_input[i] && f_isspace(new_input[i]))
            i++;
        if (is_quots(new_input[i]))                // to be implement
        
    }
	return (new_input);
}

int lexer_function(t_token  **token, char *input) // not finished
{
	char    *filtred;
	int     count;

	filtred = NULL;
	if (!token || !input || !*input)
		return (0);
	// cat abc
	filtred = filtring_input(input);            // to check input for error and align it
	if (!filtred)
		return (0);
	count = count_word(filtred);                // count word in input
	add_token(*token, count);                   // add Node to struct
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
    // t_token_type    a = WORD, b = HEREDOC, c = APPEND;
    // printf("%d %d %d\n", a, b, c);
    // printf("size of enum [%lu]\n", sizeof(a));
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
