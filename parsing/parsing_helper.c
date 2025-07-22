/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:31:11 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:41:42 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**handl_word(char **args, char *new_arg)
{
	int		len;
	char	**new_args;
	int		i;

	len = 0;
	i = 0;
	if (args)
		while (args[len])
			len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);
	while (i < len)
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

int	is_valid_redir_filename(t_token *op_token)
{
	char	*filename;

	if (!op_token || !op_token->next)
		return (write_error(4), 0);
	filename = op_token->next->token;
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
	t_redir	*temp;

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

int	create_and_add_redir(t_cmd *cmd, t_token *redir_token)
{
	t_redir	*new_redir;
	char	*filename;

	filename = redir_token->next->token;
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (0);
	new_redir->filename = ft_strdup(filename);
	if (!new_redir->filename)
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

int	handle_redirection(t_cmd *cmd, t_token **curr_token)
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
