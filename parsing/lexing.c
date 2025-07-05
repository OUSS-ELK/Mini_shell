#include "../minishell.h"

char	*extract_quoted_content(char *input, int start, int *end)
{
	int		i;
	char	*str;
	char	quote;

	quote = input[start];
	i = start + 1;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] != quote)
		return (NULL);
	str = ft_substr(input, start + 1, i - start - 1);
	if (!str)
		return (NULL);
	*end = i;
	return (str);
}

char	*handle_quote_expansion(char *str, char quote, t_env *env, bool heredoc)
{
	char	*expand;

	if (heredoc || quote == '\'')
		expand = ft_strdup(str);
	else
		expand = expand_var_str(str, env, heredoc);
	return (expand);
}


int inside_quote(char *input, int start, char **output, t_env *env, bool *heredoc)
{
	char	*str;
	char	*expand;
	int		end;
	char	quote;

	quote = input[start];
	str = extract_quoted_content(input, start, &end);
	if (!str)
		return (-1);
	expand = handle_quote_expansion(str, quote, env, *heredoc);
	free(str);
	if (!expand)
		return (-1);
	*output = expand;
	return (end + 1);
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

void add_token(t_token **token, t_token_vars *vars)
{
	t_token	*new;
	t_token	*tmp;

	if (!vars || !vars->value)
		return ;
	new = creat_token(vars->value, vars->type, vars->space, vars->quoted);
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

static t_token_type	get_opr_type(char *input, int i)
{
	if (input[i] == '>' && input[i + 1] == '>')
		return (APPEND);
	if (input[i] == '<' && input[i + 1] == '<')
		return (HEREDOC);
	if (input[i] == '>')
		return (REDIR_OUT);
	if (input[i] == '<')
		return (REDIR_IN);
	if (input[i] == '|')
		return (PIPE);
	return (0);
}

int	check_operator(t_oprvars *op_vars)
{
	t_token_type	type;
	t_token_vars	vars;

	type = get_opr_type(op_vars->input, op_vars->i);
	vars.type = type;
	vars.space = op_vars->space;
	vars.quoted = false;
	if (type == APPEND || type == HEREDOC)
	{
		if (type == HEREDOC)
			*(op_vars->heredoc) = true;
		vars.value = ft_substr(op_vars->input, op_vars->i, 2);
		add_token(op_vars->token, &vars);
		free(vars.value);
		return (op_vars->i + 2);
	}
	vars.value = ft_substr(op_vars->input, op_vars->i, 1);
	add_token(op_vars->token, &vars);
	free(vars.value);
	return (op_vars->i + 1);
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
