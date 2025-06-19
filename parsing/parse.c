#include "../minishell.h"

int is_oper(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == HEREDOC);
}

t_cmd	*alloc_new_cmd(void)
{
	t_cmd *new;

	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
}

char	**handle_word(char **args, char *new_arg)
{
	int		len = 0;
	char	**new_args;
	int 	i = 0;

	// printf("inside add_to_arg\n");
	if (args)
		while (args[len])
			len++;
	// print_array(args);
	// printf("len = %d\n", len);

	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);

	while ( i < len)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[len] = ft_strdup(new_arg);
	if (!new_args[len])
		return (free(new_args), NULL);
	new_args[len + 1] = NULL;
	free(args);
	return (new_args);
}

// void handle_word(t_cmd *cmd, t_token *token)
// {
// 	cmd->args = add_to_args(cmd->args, token->token);
// }

void	add_redirection(t_redir **redir_list, t_redir *new_redir)
{
	t_redir *temp;

	if (!*redir_list)
	{
		*redir_list = new_redir;
		return;
	}
	temp = *redir_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_redir;
}

int handle_redirection(t_cmd *cmd, t_token **curr_token)
{
    t_redir	*new_redir;

    if (!(*curr_token)->next || (*curr_token)->next->type != WORD)
    {
        write_error(4);
        return 0;
    }
	new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
		return 0;
    new_redir->filename = ft_strdup((*curr_token)->next->token);
    if (!new_redir->filename)
    {
		printf("error filename\n");
        free(new_redir);
        return 0;
    }
    new_redir->type = (*curr_token)->type;
  
    new_redir->next = NULL;
    add_redirection(&cmd->redir, new_redir);
    *curr_token = (*curr_token)->next;
    return (1);
}


t_cmd *parse_cmd(t_token **token)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token *curr_token;

	if (token)
		printf("NULL\n");
	head = alloc_new_cmd();
	curr = head;
	curr_token = *token;

	while (curr_token)
	{
		if (curr_token->type == PIPE)
		{
		// printf("_PIPE  token->type = %d\n", curr_token->type);
			if (!curr->args && !curr->redir)
			{
				write_error(5);
				free_cmd(head);
				return (NULL);
			}
			curr->next = alloc_new_cmd();
			if (!curr->next)
			{
                free_cmd(head);
                return NULL;
            }
			curr = curr->next;
			curr_token = curr_token->next;
		}
		else if (is_oper(curr_token->type))
		{
		// printf("_Operators token->type = %d\n", curr_token->type);
			if (!handle_redirection(curr, &curr_token))
			{
				free_cmd(head);
				return (NULL);
			}
			curr_token = curr_token->next;
		}
		else if (curr_token->type == WORD)
		{
		// printf(" _WORDS token->type = %d\n", curr_token->type);
			if (!curr->redir || curr->redir->filename)
				curr->args = handle_word(curr->args, curr_token->token);
			curr_token = curr_token->next;
		}
		else
			curr_token = curr_token->next;
	}
	if (!head->args)
	{
       free_cmd(head);
       return(NULL);
	}
	return (head);
}
