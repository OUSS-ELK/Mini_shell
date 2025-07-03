#include "../minishell.h"

// find the closing quote & return index & expand inside Double_quote
int	inside_quote(char *input, int start, char **output, t_env *env, bool *heredoc)
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
	if (*heredoc)
	{
		expand = ft_strdup(str);
		if (!expand)
			return (free(str), -1);
	}
	else if (quote == '"')													// Expanding inside DQ
	{
		expand = expand_var_str(str, env, *heredoc);
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

t_token	*creat_token(char *input, t_token_type type, bool space, bool quote)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token = ft_strdup(input);
	new->type = type;
	new->space = space;
	new->quote = quote;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **token, char *input, t_token_type type, bool space, bool quote)
{
	t_token	*new;
	t_token	*tmp;

	new = creat_token(input, type, space, quote);
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

int	check_operator(char *input, int i, t_token **token, bool space, bool *heredoc)
{
	t_token_type	type;

	// printf("inside check operator\n");
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
		if (type == HEREDOC)
			*heredoc = true;
	// printf("heredoc flag = %d\n", *heredoc);
		add_token(token, ft_substr(input, i, 2), type, space, false);
		// *heredoc = false;
		return (i + 2);
	}
	add_token(token, ft_substr(input, i, 1), type, space, false);
	return (i + 1);
}

void	merge_words(t_token **token)                            		     // function to handle mixed word (if space true d'ont merge)
{
	t_token	*curr;
	t_token *tmp;
	char	*merged;

	curr = *token;
	if (!curr || !curr->next)
		return ;
	// printf("inside merge word\n");
	while (curr && curr->next)
	{
		// should merge if no space after word.
		if (curr->type == WORD && curr->next->type == WORD && curr->next->space == false)
		{
			// printf("mix \n");
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
