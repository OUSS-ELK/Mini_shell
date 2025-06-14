#include "../minishell.h"

int	is_oper_type(t_token *curr_token)
{
	return (curr_token->type == REDIR_IN  || curr_token->type == REDIR_OUT || curr_token->type == APPEND || curr_token->type == HEREDOC);
}

t_cmd   *alloc_new_cmd(void)
{
    t_cmd   *new;

    new = malloc(sizeof(t_cmd));
    if (!new)
        return (NULL);
    new->cmd = NULL;
    new->args = NULL;
    new->redir = NULL;
    new->next = NULL;
    return (new);
}

t_cmd *parse_cmd(t_token **token)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*curr_token;

	head = NULL;
	curr = NULL;
	curr_token = *token;
	while (curr_token)
	{
		if (!curr)
			curr = alloc_new_cmd();
		if (curr_token->type == WORD)
			handle_word(curr, curr_token);
		else if (is_oper_type(curr_token))
		{
			if (is_redirection(curr_token->token))
				handle_redirection(curr, &curr_token);
			else if (curr_token->type == PIPE)
			{
				curr->next = alloc_new_cmd();
				curr = curr->next;
			}
		}
		curr_token = curr_token->next;
	}
	return head;
}
