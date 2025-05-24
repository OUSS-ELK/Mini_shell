#include "minishell.h"

int find_word(char *input, int start, t_token **token)
{
    int last;

    last = start;        
    while (input[last] && is_word_start(input[last]))
        last++;
    if (last > start)
        add_token(token, f_substring(input, start, last - start), WORD);
    return (last);
}


// find the closing quote & return index after it
int	inside_quote(char *input, int start, char **output)
{
	char	quote;
	int		i;

	// printf("(inside quote function) input = [%c]\n", input[start]);
	quote = input[start];									// save the opening cote
	i = start + 1;
	// printf("(i = [%d]\n", i);
	// if ((input[i] == '"' || input[i] == '\'') && (input[i + 1] != '"' || input[i + 1] == '\''))
	// {
	// 	printf("( [input[i] = %c] && [input{i + 1}[%c]]\n", input[i], input[i + 1]);
	// 	add_token(token, "\0", WORD);
	// 	return (i + 1);
	// }
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
		return (-1);
	*output = f_substring(input, start + 1, i - start - 1);
	if (!*output)
		return (-1);
	return (i + 1);
}

t_token	*create_token(char *input, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token = input;
	new->type = type;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **token, char *input, t_token_type type)
{
	t_token	*new;
	t_token	*tmp;

	new = create_token(input, type);
	if (!new)
		return ;
	if (*token == NULL)
	{
		*token = new;
		return ;
	}
	tmp = *token;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	printf("add_token\n");
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
		type = 0;
	if (type == APPEND || type == HEREDOC)
	{
		// printf("append or heredoc\n");
		add_token(token, f_substring(input, i, 2), type);
		return (i + 2);
	}
	add_token(token, f_substring(input, i, 1), type);
	return (i + 1);
}
