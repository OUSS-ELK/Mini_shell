/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:38 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:35:41 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*alloc_new_cmd(void)
{
	t_cmd	*new;

	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
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

int	is_mergeable_words(t_token *token)
{
	t_token	*curr;

	curr = token;
	if (!curr || !curr->next)
		return (0);
	while (curr && curr->next)
	{
		if (curr->type == WORD && curr->next->type == WORD
			&& curr->next->space == false)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	merge_words(t_token **token)
{
	t_token	*curr;
	t_token	*tmp;
	char	*merged;

	curr = *token;
	while (curr && curr->next)
	{
		if (curr->type == WORD && curr->next->type == WORD
			&& curr->next->space == false)
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
