#include "../minishell.h"

void	write_error(int	n)
{
	if (n == 1)
		write(2, "Parsing Error\n", 14);
	else if (n == 2)
		write(2, "Lexer Error\n", 12);
	else if (n == 3)
		write(2, "Quote Error\n", 12);
	else if (n == 4)
		write(2, "syntax error: missing filename after redirection\n", 49);
	else if (n == 5)
		write(1, "Syntax error empty command before pipe\n", 39);
	else if (n == 6)
		write(1, "Syntax error empty command after pipe\n", 38);
	else if (n == 7)
		write(1, "Syntax error ambiguous redirect\n", 32);
	else
		write(2, "Error\n", 6);
}

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

t_token	*get_last_token(t_token *token)
{
	if (!token)
		return (NULL);

	while (token->next)
		token = token->next;
	return (token);
}

int	all_space(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (1);
	while (input[i])
	{
		if (!f_isspace(input[i]) && input[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

int f_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_quote(char quote)
 {	
	return (quote == '\'' || quote == '"');
}

int	is_operator(char oper)
{
	return (oper == '|' || oper == '>' || oper == '<');
}

int	is_word_start(char c)
{
	return (!f_isspace(c) && !is_operator(c) && !is_quote(c) && c != '$');
}

int	is_alpha(char input)
{
	return ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'));
}

int	valid_expand(char input, char next)
{
	return (input == '$' && (ft_isalnum(next) || next == '{' || next == '_' || next == '?' || next == '$'));
}

void	print_env(t_env *env)
{
	while (env)
	{
		printf(BLUE"KEY [%s] | VALUE [%s] | \n" RESET, env->key, env->value);
		env = env->next;
	}
}

void	print_tokens(t_token *token)
{
	while (token)
	{
		printf(BLUE"TOKEN [%s] | TYPE [%d] | space[%d]\n" RESET, token->token, token->type, token->space);
		token = token->next;
	}
}

void    ll(void)
{
    system("leaks -q minishell");
}

int	check_quote(char *input)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !dq)
			sq = !sq;
		else if (input[i] == '"' && !sq)
			dq = !dq;
		i++;
	}
	if (sq || dq)
		return (0);
	return (1);
}

void	print_array(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		printf("arr[%s]\n", arr[i]);
		i++;
	}
}

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
			printf(GREEN"Redir: type[%d], filename [%s]\n"RESET, r->type, r->filename);
			r = r->next;
		}
		cmd = cmd->next;
	}
}
