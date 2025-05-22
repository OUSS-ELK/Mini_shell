#include "minishell.h"

// int	quote_type(char	q)
// {
// 	if (q == '\'')
// 		return (1);
// 	else if (q == '\"')
// 		return (2);
// 	else
// 		return (0);
// }

void	write_error(int	n)
{
	if (n == 1)
		write(2, "Error in Parsing\n", 17);
	else
		write(2, "Error\n", 6);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	while (token)
	{
		tmp = token->next;
		free(token);
		token = tmp;
	}
}

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

int f_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_quote(char quote)
{
	if (quote == '\'' || quote == '"')
		return (1);
	return (0);
}

// find the closing quote & return index after it
int	inside_quote(char *input, int start, t_token *token)
{
	char	quote;
	int		q;

	q = 0;
	quote = input[start];							// save the opening cote
	start++;
	// printf("(inside quote) input = [%c]\n", input[start]);
	while (input[start])
	{
	// printf("(while) input = [%c]\n", input[start]);
		if (input[start] == quote && !q)			// found match closing quote
		{
			// printf("(found match [quote = %c] == [input[%c]]\n", quote, input[start]);
			q = 1;
			return (start + 1);
		}
		start++;
	}
	return (-1);									// no match
}

int	is_operator(char oper)
{
	return (oper == '|' || oper == '>' || oper == '<');
}

int	check_operator(char *input, int i, t_token **token)
{
	t_token_type	type;

	if (input[i] == '>' && input[i + 1] == '>')
		type = APPEND;
	else if (input[i] == '<' && input[i + 1] == '<')
		type = HEREDOC;
	else if (input[i] == '>')
		type = REDIR_OUT;
	else if (input[i] == '<')
		type = REDIR_IN;
	else if (input[i] == '|')
		type = PIPE;
	else
		return (i);
	if (type == APPEND || type == HEREDOC)
	{
		add_token(token, f_substring(input, i, 2), type);
		return (i + 2);
	}
	add_token(token, f_substring(input, i, 1), type);
	return (i + 1);

int	lexer_input(t_token *token, char *input)         // not finished
{
	t_token_type	type;
    int	i;
	
	// skip space
	// checking closed quotes
	// "word  > ("") not closed error.
	// inside quots ("hello world") => one word
    i = 0;
    while (input[i])
    {
		// printf("(inside lexer) input = [%c]\n", input[i]);
        if (input[i] && f_isspace(input[i]))		// skip white space if not quote
            i++;
        else if (is_quote(input[i]))				// handle quotes errors
		{
			// printf(" when found quote [i = %d] | [start = %d]\n", i, start);
			i = inside_quote(input, i, token);
			// printf(" after quote [i = %d]\n", i);
			if (i == -1)
				return (0);
		}
		else if (is_operator(input[i]))
			i = check_operator(input, i, token);
		else 
			i = find_word(input, i, token);
    }
	return (1);
}


int parsing_function(t_token  **token, char *input)			// not finished
{
	// int     count;

	if (!token || !input || !*input)
		return (0);

	if (!lexer_input(*token, input))            		// check input for error 
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

int main(void)
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
				// printf("add_history\n");
			}
			printf("input reading by readline [%s]\n", input);
			if (parsing_function(&token, input) == 0)
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
