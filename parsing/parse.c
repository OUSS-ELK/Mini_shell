/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oussama-elk <oussama-elk@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:28 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/21 00:52:29 by oussama-elk      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int handle_pipe(t_cmd **curr, t_token **curr_token, t_cmd **head)
{
	printf("INSIDE handle Pipe\n");
	if (!(*curr)->args && !(*curr)->redir)
		return (write_error(5), 0);
	if (!(*curr_token)->next)
		return (write_error(6), 0);
	(*curr)->next = alloc_new_cmd();
	if (!(*curr)->next)
		return (free_cmd(*head), 0);
	*curr = (*curr)->next;
	*curr_token = (*curr_token)->next;
	return (1);
}

int handle_word_token(t_cmd *curr, t_token **curr_token)
{
	printf("INSIDE handle word token\n");
	if (!curr->redir || (curr->redir && curr->redir->filename))
	{
		curr->args = handl_word(curr->args, (*curr_token)->token);
		if (!curr->args)
			return (0);
	}
	*curr_token = (*curr_token)->next;
	return (1);
}

int handle_cmd(t_cmd **curr, t_token **curr_token, t_cmd **head)
{
	printf("INSIDE (while) handle cmd\n");
	if ((*curr_token)->type == PIPE)
		return (handle_pipe(curr, curr_token, head));
	else if (is_oper((*curr_token)->type))
		return (handle_redirection(*curr, curr_token));
	else if ((*curr_token)->type == WORD)
		return (handle_word_token(*curr, curr_token));
	else
		*curr_token = (*curr_token)->next;
	return (1);
}

int init_parse_vars(t_cmd **head, t_cmd **curr)
{
	*head = alloc_new_cmd();
	if (!*head)
		return (0);
	*curr = *head;
	return (1);
}

t_cmd	*parse_cmd(t_token **token)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*curr_token;

	if (!token)
		return (NULL);
	printf("INSIDE PARSE CMD\n");
	if (!init_parse_vars(&head, &curr))
		return (NULL);
	curr_token = *token;
	while (curr_token)
	{
		if (!handle_cmd(&curr, &curr_token, &head))
		{
			free_cmd(head);
			return (NULL);
		}
	}
	if (!head->args && !head->redir)
	{
		free_cmd(head);
		return (NULL);
	}
	return (head);
}
