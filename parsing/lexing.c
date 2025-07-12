#include "../minishell.h"

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

void	add_token(t_token **token, t_token_vars *vars)
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

void	merge_words(t_token **token)                            		     // function to handle mixed word (if space true d'ont merge)
{
	t_token	*curr;
	t_token *tmp;
	char	*merged;

	curr = *token;
	if (!curr || !curr->next)
		return ;
	while (curr && curr->next)
	{
		if (curr->type == WORD && curr->next->type == WORD && curr->next->space == false)
		{
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
