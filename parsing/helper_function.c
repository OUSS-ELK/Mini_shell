/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 19:33:43 by ouelkhar          #+#    #+#             */
/*   Updated: 2025/07/22 02:29:50 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int f_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int is_oper(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == HEREDOC);
}

int	is_operator(char oper)
{
	return (oper == '|' || oper == '>' || oper == '<');
}

int	is_quote(char quote)
{	
	return (quote == '\'' || quote == '"');
}

// void	print_env(t_env *env)
// {
// 	while (env)
// 	{
// 		printf(BLUE"KEY [%s] | VALUE [%s] | \n" RESET, env->key, env->value);
// 		env = env->next;
// 	}
// }

// void	print_tokens(t_token *token)
// {
// 	while (token)
// 	{
// 		printf(BLUE"TOKEN [%s] | TYPE [%d] | space[%d] | quote[%d]\n" RESET, token->token, token->type, token->space, token->quote);
// 		token = token->next;
// 	}
// }

// void	print_array(char **arr)
// {
// 	int i = 0;
// 	while (arr[i])
// 	{
// 		printf("arr[%s]\n", arr[i]);
// 		i++;
// 	}
// }

void	print_cmds(t_cmd *cmd)
{
	int i = 0;
	while (cmd)
	{
		printf(RED"==== CMD ====\n"RESET);
		if (cmd->args)
		{
			printf(YELLOW"Args:\n"RESET);
			while (cmd->args[i])
				printf(BLUE"  [%s]\n"RESET, cmd->args[i++]);
			i = 0;
		}

		t_redir *r = cmd->redir;
		while (r)
		{
			printf(GREEN"Redir: type[%d], filename [%s] | quote[%d]\n"RESET, r->type, r->filename, r->quoted);
			r = r->next;
		}
		cmd = cmd->next;
	}
}
