#include "../minishell.h"

void	write_error(int	n)
{
	if (n == 1)
		write(2, "Parsing Error\n", 14);
	else if (n == 2)
		write(2, "Lexer Error\n", 12);
	else if (n == 3)
		write(2, "Quote Error\n", 12);
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
	return (!f_isspace(c) && !is_operator(c) && !is_quote(c));
}

int	is_alpha(char input)
{
	return ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'));
}

int	valid_expand(char input, char next)
{
	return (input == '$' && (is_alpha(next) || next == '{' || next == '_' || next == '?' || next == '$'));
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
		printf(BLUE"TOKEN [%s] | TYPE [%d] | \n" RESET, token->token, token->type);
		token = token->next;
	}
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

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	char	**str;				// str[0] = key & str[1] = value
	int		i;

	if (!env)
		return (NULL);
	
	top = malloc(sizeof(t_env));
	if (!top)
		return (NULL);
	curr = top;
	i = 0;
	while (env[i])
	{
		str = ft_split(env[i], '=');
		if (!str)
		{
			free_env(top);
			return (NULL);
		}
		curr->key = ft_strdup(str[0]);
		curr->value = ft_strdup(str[1] ? str[1] : "");
		free_array(str);
		if (!env[i + 1])
			curr->next = NULL;
		else
		{
			curr->next = malloc(sizeof(t_env));
			if (!curr->next)
			{
				free_env(top);
				return (NULL);
			}
			curr = curr->next;
		}
		i++;
	}
	return (top);
}

char	*ft_getenv(char *key, t_env *env)
{
	while (env != NULL)
	{
		if (ft_strncmp(key, env->key, ft_strlen(key)) == 0 && ft_strlen(key) == ft_strlen(env->key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*f_strjoin_free(char *s1, char *s2)
{
	char 	*joined;
	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	*f_strjoin_char(char *s, char c)
{
	char str[2] = {c, 0};

	return f_strjoin_free(s, str);
}

char	*expand_var_str(char *str, t_env *env)   // just to test (copied) should rebuild it   | add check for '${USER'} => sould expand
{
	char	*result;
	int		i;
	char	*tmp;
	int		start;
	char	*value;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			if (str[i] == '?')
			{
				tmp = ft_itoa(0);
				result = f_strjoin_free(result, tmp);
				free(tmp);
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				start = i;
				while (ft_isalnum(str[i]) || str[i] == '_')
					i++;
				tmp = ft_substr(str, start, i - start);
				value = ft_getenv(tmp, env);
				if (value)
					result = f_strjoin_free(result, value);
				else
					result = f_strjoin_free(result, "");
				free(tmp);
			}
			else
				result = f_strjoin_char(result, '$');
		}
		else
		{
			result = f_strjoin_char(result, str[i]);
			i++;
		}
	}
	return (result);
}
