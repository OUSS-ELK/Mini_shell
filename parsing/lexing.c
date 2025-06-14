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

// find the closing quote & return index & expand inside Double_quote
int	inside_quote(char *input, int start, char **output, t_env *env)
{
	char	quote;
	char	*expand;
	char	*str;
	int		i;

	// printf(RED"(inside quote function) input = [%c] | start = [%d]\n" RESET, input[start], start);
	quote = input[start];												// Save the opening quote
	i = start + 1;
	while (input[i] && input[i] != quote)
		i++;
	str = ft_substr(input, start + 1, i - start - 1);					// Take what inside quotes
	if (!str)
		return (-1);
	if (quote == '"')													// Expanding inside DQ
	{
		expand = expand_var_str(str, env);
		if (!expand)
			return (free(str), -1);
	}
	else																// Normal word inside quotes
	{
		expand = ft_strdup(str);
		if (!expand)
			return (free(str), -1);
	}
	free(str);
	*output = expand;
	return (i + 1);
}

t_token	*creat_token(char *input, t_token_type type, bool space)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token = ft_strdup(input);
	new->type = type;
	new->space = space;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **token, char *input, t_token_type type, bool space)
{
	t_token	*new;
	t_token	*tmp;

	new = creat_token(input, type, space);
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
}

int	check_operator(char *input, int i, t_token **token, bool space)
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
		add_token(token, ft_substr(input, i, 2), type, space);
		return (i + 2);
	}
	add_token(token, ft_substr(input, i, 1), type, space);
	return (i + 1);
}

void	merge_words(t_token **token)                            		     // function to handle mixed word (should fix some cases)
{
	t_token	*curr;
	t_token *tmp;
	char	*merged;

	curr = *token;
	if (!curr && !curr->next)
		return ;
	while (curr && curr->next)
	{
		if (curr->type == WORD && curr->next->type == WORD && curr->next->space == false)
		{
			// should merge if not space after word.
			merged = ft_strjoin(curr->token, curr->next->token);
			if (!merged)
				return ;
			free(curr->token);
			curr->token = merged;

			tmp = curr->next;
			curr->next = tmp->next;
			free(tmp->token);
			free(tmp);
		}
		else
			curr = curr->next;
	}
}

