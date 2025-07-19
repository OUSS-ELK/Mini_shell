/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:38 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/19 19:35:27 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_token_type	get_opr_type(char *input, int i)
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
