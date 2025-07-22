#include "../minishell.h"

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
			free_array(cmd->args);
		if (cmd->redir)
			free_redir(cmd->redir);
		free(cmd);
		cmd = tmp;
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_array(char **str)
{
	char	**tmp;

	if (!str)
		return ;
	tmp = str;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(str);
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	while (token)
	{
		tmp = token->next;
		if (token->token)
			free(token->token);
		free(token);
		token = tmp;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}
