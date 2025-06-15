#include "../minishell.h"

int is_operator(t_token *curr_token)
{
	return (curr_token->type == REDIR_IN || curr_token->type == REDIR_OUT || curr_token->type == APPEND || curr_token->type == HEREDOC);
}

t_cmd *alloc_new_cmd(void)
{
	t_cmd *new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd = NULL;
	new->args = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
}

void	handle_redirection(t_cmd *cmd, t_token **curr_token)
{
	t_redir	*new_redir;
	int		type;

	if (!(*curr_token) || !(*curr_token)->next)
	{
		write_error(4);
		return ;
	}
	type = get_type((*curr_token)->type);
	(*curr_token) = (*curr_token)->next;
	if ((*curr_token) != WORD)
	{
		write_error(5);
		return ;
	}
	new_redir = malloc(sizeof(curr_token));
	if (!new_redir)
		return ;
	new_redir->type = type;
	new_redir->filename = ft_strdup((*curr_token)->token);
	add_redirection(&(cmd->redir), new_redir);
}

void handle_word(t_cmd *cmd, t_token *token)
{
	if (!cmd->cmd)
		cmd->cmd = ft_strdup(token->token);
	cmd->args = add_to_args(cmd->args, token->token);
}

t_cmd *parse_cmd(t_token **token)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token *curr_token;

	head = alloc_new_cmd();
	curr = head;
	curr_token = *token;
	while (curr_token)
	{
		if (curr_token->type == PIPE)
		{
			curr->next = alloc_new_cmd();
			curr = curr->next;
		}
		else if (curr_token->type == WORD)
			handle_word(curr, curr_token);
		else if (is_redirection(curr_token->token))
			handle_redirection(curr, &curr_token);
		curr_token = curr_token->token;
	}
	return (head);
}
