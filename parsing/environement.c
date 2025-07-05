#include "../minishell.h"

int expand_exit_status(t_token **token, bool *space)
{
	t_token_vars	vars;
	char			*expanded;

	expanded = ft_itoa(0);
	if (!expanded)
		return (-1);
	vars.value = expanded;
	vars.type = WORD;
	vars.space = *space;
	vars.quoted = false;
	add_token(token, &vars);
	*space = false;
	free(expanded);
	return (0);
}

int expand_bracket_var(t_token **token, char *input, int start, t_env *env, bool *space)
{
	t_token_vars	vars;
	int				len = 0;
	char			*var_name;
	char			*expanded;

	while (input[start + len] && input[start + len] != '}')
		len++;
	if (input[start + len] != '}')
		return (-1);
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (-1);
	expanded = ft_getenv(var_name, env);
	free(var_name);
	if (!expanded)
		expanded = "";
	vars.value = ft_strdup(expanded);
	vars.type = WORD;
	vars.space = *space;
	vars.quoted = false;
	add_token(token, &vars);
	*space = false;
	free(vars.value);
	return (start + len + 1);
}

int expand_simple_var(t_token **token, char *input, int start, t_env *env, bool *space)
{
	t_token_vars	vars;
	int				len = 0;
	char			*var_name;
	char			*expanded;

	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start + len] == '_'))
		len++;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (-1);
	expanded = ft_getenv(var_name, env);
	free(var_name);
	if (!expanded)
		expanded = "";
	vars.value = ft_strdup(expanded);
	vars.type = WORD;
	vars.space = *space;
	vars.quoted = false;
	add_token(token, &vars);
	*space = false;
	free(vars.value);
	return (start + len - 1);
}

int expanding_var(t_expndvars *exp_var)
{
	int	start;
	int	ret;

	start =exp_var->heredoc ? exp_var->i : exp_var->i + 1;
	if (exp_var->input[start] == '?')
	{
		ret = expand_exit_status(exp_var->token,exp_var->space);
		if (ret == -1)
			return (-1);
		return (start + 1);
	}
	if (exp_var->input[start] == '{')
	{
		ret = expand_bracket_var(exp_var->token,exp_var->input, start + 1,exp_var->env,exp_var->space);
		if (ret == -1)
			return (-1);
		return (ret + 1);
	}
	ret = expand_simple_var(exp_var->token,exp_var->input, start,exp_var->env,exp_var->space);
	if (ret == -1)
		return (-1);
	return (ret + 1);
}

static t_env	*new_env_node(char *env_str)
{
	t_env	*node;
	char	**str;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	str = ft_split(env_str, '=');
	if (!str)
	{
		free(node);
		return (NULL);
	}
	node->key = ft_strdup(str[0]);
	node->value = ft_strdup(str[1] ? str[1] : "");
	free_array(str);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free_env(node);
		return (NULL);
	}
	return (node);
}

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	t_env	*new_node;
	int		i;

	if (!env)
		return (NULL);
	top = new_env_node(env[0]);
	if (!top)
		return (NULL);
	curr = top;
	i = 1;
	while (env[i])
	{
		new_node = new_env_node(env[i]);
		if (!new_node)
		{
			free_env(top);
			return (NULL);
		}
		curr->next = new_node;
		curr = new_node;
		i++;
	}
	return (top);
}

char	*ft_getenv(char *key, t_env *env)
{
	int	key_len;

	key_len = ft_strlen(key);
	while (env != NULL)
	{
		if (ft_strncmp(key, env->key, key_len) == 0 && key_len == (int)ft_strlen(env->key))
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

char	*expand_bracket_qt(t_exp_strvars *exp_var)
{
	char	*tmp;
	char	*value;
	int		start;

	start = ++exp_var->i;
	while (exp_var->str[exp_var->i] && exp_var->str[exp_var->i] != '}')
		exp_var->i++;
	if (exp_var->str[exp_var->i] != '}')
		return (exp_var->result);
	tmp = ft_substr(exp_var->str, start, exp_var->i - start);
	value = ft_getenv(tmp, exp_var->env);
	exp_var->result = f_strjoin_free(exp_var->result, value ? value : "");
	free(tmp);
	exp_var->i++;
	return (exp_var->result);
}

char	*expand_simple_qt(t_exp_strvars *exp_var)
{
	char	*tmp;
	char	*value;
	int		start;

	start = exp_var->i;
	while (ft_isalnum(exp_var->str[exp_var->i]) || exp_var->str[exp_var->i] == '_')
		exp_var->i++;
	tmp = ft_substr(exp_var->str, start, exp_var->i - start);
	value = ft_getenv(tmp, exp_var->env);
	exp_var->result = f_strjoin_free(exp_var->result, value ? value : "");
	free(tmp);
	return (exp_var->result);
}

char	*exit_status(char *result, int *i)
{
	char *tmp;

	tmp = ft_itoa(0);
	result = f_strjoin_free(result, tmp);
	free(tmp);
	(*i)++;
	return (result);
}

void	init_expand_var(t_exp_strvars *exp_var, char *str, t_env *env, bool heredoc, char *result)
{
	exp_var->str = str;
	exp_var->i = 0;
	exp_var->env = env;
	exp_var->result = result;
	exp_var->heredoc = heredoc;
}

char	*expand_loop(char *str, t_env *env, bool heredoc, char *result)
{
	t_exp_strvars	exp_var;

	init_expand_var(&exp_var, str, env, heredoc, result);
	while (exp_var.str[exp_var.i])
	{
		if (exp_var.str[exp_var.i] == '$')
		{
			exp_var.i++;
			if (exp_var.heredoc)
				exp_var.i--;
			else if (exp_var.str[exp_var.i] == '?')
				exp_var.result = exit_status(exp_var.result, &exp_var.i);
			else if (exp_var.str[exp_var.i] == '{')
				exp_var.result = expand_bracket_qt(&exp_var);
			else if (ft_isalpha(exp_var.str[exp_var.i]) || exp_var.str[exp_var.i] == '_')
				exp_var.result = expand_simple_qt(&exp_var);
			else
				exp_var.result = f_strjoin_char(exp_var.result, '$');
		}
		else
			exp_var.result = f_strjoin_char(exp_var.result, exp_var.str[exp_var.i++]);
	}
	return (exp_var.result);
}

char	*expand_var_str(char *str, t_env *env, bool heredoc)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (expand_loop(str, env, heredoc, result));
}
