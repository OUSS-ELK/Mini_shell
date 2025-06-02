#include "../minishell.h"

// int find_word(char *input, int start, t_token **token)
// {
//     int last;

//     last = start;        
//     while (input[last] && is_word_start(input[last]))
//         last++;
//     if (last > start)
//         add_token(token, f_substring(input, start, last - start), WORD);
//     return (last);
// }

// find the closing quote & return index after it
int	inside_quote(char *input, int start, char **output)  				//, t_env env, int last_exit)
{
	char	quote;
	// char	*expand;
	int		i;

	printf(RED"(inside quote function) input = [%c] | start = [%d]\n" RESET, input[start], start);
	quote = input[start];												// Save the opening quote
	i = start + 1;
	while (input[i] && input[i] != quote) 								// Skip charachters inside quotes and keep check for $ to expand its value 
	{
		// if (check_env(input + i))									// Check for Name validation
		// 	i = expand_value();
		// else
		i++;
	}
	if (!input[i])
		return (-1);
	*output = f_substring(input, start + 1, i - start - 1);
	if (!*output)
		return (-1);
	// if (quote == '"')
	// {
	// 	expand = expand_env(*output, env, last_exit);
	// 	if (!expand)
	// 		return (-1);
	// 	free(*output);
	// 	*output = expand;
	// }
	return (i + 1);
}

t_token	*creat_token(char *input, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token = f_strdup(input);
	new->type = type;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **token, char *input, t_token_type type)
{
	t_token	*new;
	t_token	*tmp;

	new = creat_token(input, type);
	if (!new)
		return ;
	if (*token == NULL)
	{
		printf("first_token\n");
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

// t_token	*new_token(t_token **token)
// {
// 	char	*n_content;
// 	t_token	*current;
// 	t_token	*next;

// 	current = *token;
// 	while (current && current->next)
// 	{
// 		if (current->space == 1)
// 		{
// 			next = current->next;
// 			n_content = f_strjoin(current->token, next->token);
// 			printf(GREEN"new_token = %s\n"RESET, n_content);
// 			if (!n_content)
// 				return (0);
// 			free(current->token);							// free old content
// 			current->token = n_content;						// remove next token from the list
// 			free(next->token);								// free next token's content
// 			free(next);										// free next token
// 		}
// 		else
// 			current = current->next;
// 	}
// 	print_tokens(*token);
// 	return (*token);
// }
