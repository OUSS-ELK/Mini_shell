#include "../minishell.h"

int	is_only_space(char *filename)
{
	int	i;

	i = 0;
	while (filename[i])
	{
		if (f_isspace(filename[i]))
			return (0);
		i++;
	}
	return (1);
}

int is_oper(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == HEREDOC);
}

t_cmd	*alloc_new_cmd(void)
{
	t_cmd *new;

	// printf("alloc new cmd\n");
	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
}

char	**handl_word(char **args, char *new_arg)
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
		new_args[i] = ft_strdup(args[i]);
		if (!new_args[i])
			return (free_array(new_args), NULL);
		i++;
	}
	new_args[len] = ft_strdup(new_arg);
	if (!new_args[len])
		return (free(new_args), NULL);
	new_args[len + 1] = NULL;
	free_array(args);
	return (new_args);
}


int is_valid_redir_filename(t_token *op_token)
{
	char *filename = op_token->next->token;

	if (!filename || filename[0] == '\0')
	{
		if (op_token->type != HEREDOC)
		{
			write_error(7);
			return (0);
		}
	}
	return (1);
}

void	add_redirection(t_redir **redir_list, t_redir *new_redir)
{
	t_redir *temp;

	if (!*redir_list)
	{
		*redir_list = new_redir;
		return ;
	}
	temp = *redir_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_redir;
}

int create_and_add_redir(t_cmd *cmd, t_token *redir_token)
{
	t_redir *new_redir;
	char *filename = redir_token->next->token;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (0);
	new_redir->filename = ft_strdup(filename);
	if (!new_redir->filename || !is_only_space(new_redir->filename))
	{
		free(new_redir);
		return (0);
	}
	new_redir->type = redir_token->type;
	new_redir->quoted = redir_token->next->quote;
	new_redir->next = NULL;
	add_redirection(&cmd->redir, new_redir);
	return (1);
}

int handle_redirection(t_cmd *cmd, t_token **curr_token)
{
	if (!(*curr_token)->next || (*curr_token)->next->type != WORD)
	{
		write_error(4);
		return (0);
	}
	if (!is_valid_redir_filename(*curr_token))
		return (0);
	if (!create_and_add_redir(cmd, *curr_token))
		return (0);
	*curr_token = (*curr_token)->next->next;
	return (1);
}


int handle_pipe(t_cmd **curr, t_token **curr_token, t_cmd **head)
{
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
	if (!curr->redir || (curr->redir && curr->redir->filename))
	{
		curr->args = handl_word(curr->args, (*curr_token)->token);
		if (!curr->args)
			return (0);
	}
	*curr_token = (*curr_token)->next;
	return (1);
}

int handle_cmd_logic(t_cmd **curr, t_token **curr_token, t_cmd **head)
{
	if ((*curr_token)->type == PIPE)
		return handle_pipe(curr, curr_token, head);
	else if (is_oper((*curr_token)->type))
		return handle_redirection(*curr, curr_token);
	else if ((*curr_token)->type == WORD)
		return handle_word_token(*curr, curr_token);
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

t_cmd *parse_cmd(t_token **token)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*curr_token;

	if (!token)
		return (NULL);
	if (!init_parse_vars(&head, &curr))
		return (NULL);
	curr_token = *token;
	while (curr_token)
	{
		if (!handle_cmd_logic(&curr, &curr_token, &head))
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

// char	**handl_word(char **args, char *new_arg)
// {
// 	int		len = 0;
// 	char	**new_args;
// 	int 	i = 0;

// 	// printf("inside add_to_arg\n");
// 	if (args)
// 		while (args[len])
// 			len++;
// 	// print_array(args);
// 	// printf("len = %d\n", len);
// 	new_args = malloc(sizeof(char *) * (len + 2));
// 	if (!new_args)
// 		return (NULL);

// 	while ( i < len)
// 	{
// 		new_args[i] = args[i];
// 		i++;
// 	}
// 	new_args[len] = ft_strdup(new_arg);
// 	if (!new_args[len])
// 		return (free(new_args), NULL);
// 	new_args[len + 1] = NULL;
// 	free(args);
// 	return (new_args);
// }

// int handle_redirection(t_cmd *cmd, t_token **curr_token)
// {
//     t_redir	*new_redir;
// 	char	*filename;

//     if (!(*curr_token)->next || (*curr_token)->next->type != WORD)
//     {
//         write_error(4);
//         return 0;
//     }
// 	filename = (*curr_token)->next->token;
// 	if (!filename || filename[0] == '\0')
// 	{
// 		if ((*curr_token)->type != HEREDOC)
// 		{
// 			write_error(7);
// 			return (0);
// 		}
// 	}
// 	new_redir = malloc(sizeof(t_redir));
//     if (!new_redir)
// 		return 0;
//     new_redir->filename = ft_strdup((*curr_token)->next->token);
//     if (!new_redir->filename || !is_only_space(filename))
//     {
// 		printf("error filename\n");
//         free(new_redir);
//         return 0;
//     }
//     new_redir->type = (*curr_token)->type;
//     new_redir->quoted = (*curr_token)->next->quote;
//     new_redir->next = NULL;
//     add_redirection(&cmd->redir, new_redir);
//     *curr_token = (*curr_token)->next->next;
//     return (1);
// }

// t_cmd *parse_cmd(t_token **token)
// {
// 	t_cmd	*head;
// 	t_cmd	*curr;
// 	t_token *curr_token;

// 	if (!token)
// 		return (NULL);
// 	head = alloc_new_cmd();
// 	curr = head;
// 	curr_token = *token;

// 	while (curr_token)
// 	{
// 		if (curr_token->type == PIPE)
// 		{
// 		// printf("_PIPE  token->type = %d\n", curr_token->type);
// 			if (!curr->args && !curr->redir)
// 			{
// 				write_error(5);
// 				free_cmd(head);
// 				return (NULL);
// 			}
// 			if (!curr_token->next)
// 			{
// 				write_error(6);
// 				free_cmd(head);
// 				return (NULL);
// 			}
// 			if (curr_token->next)
// 			{
// 				curr->next = alloc_new_cmd();
// 				if (!curr->next)
// 				{
// 					free_cmd(head);
// 					return NULL;
// 				}
// 			}
// 			curr = curr->next;
// 			curr_token = curr_token->next;
// 		}
// 		else if (is_oper(curr_token->type))
// 		{
// 		// printf("_Operators token->type = %d\n", curr_token->type);
// 			if (!handle_redirection(curr, &curr_token))
// 			{
// 				free_cmd(head);
// 				return (NULL);
// 			}
// 			// curr_token = curr_token->next;
// 		}
// 		else if (curr_token->type == WORD)
// 		{
// 		// printf(" _WORDS token->type = %d\n", curr_token->type);
// 			if (!curr->redir || (curr->redir && curr->redir->filename))
// 			{
// 				// printf("deb\n");
// 				curr->args = handl_word(curr->args, curr_token->token);
// 				curr_token = curr_token->next;
// 			}
// 		}
// 		else
// 			curr_token = curr_token->next;
// 	}
// 	if (!head->args && !head->redir)
// 	{
//        free_cmd(head);
//        return(NULL);
// 	}
// 	return (head);
// }
