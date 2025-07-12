#include "../minishell.h"

t_env	*new_env_node(char *env_str)
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
	char	str[2];
	
	str[0] = c;
	str[1] = 0;
	return (f_strjoin_free(s, str));
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

char	*handle_dollar_expansion(t_exp_strvars *exp_var)
{
	exp_var->i++;
	if (exp_var->heredoc)
		exp_var->i--;
	else if (exp_var->str[exp_var->i] == '?')
		exp_var->result = exit_status(exp_var->result, &exp_var->i);
	else if (exp_var->str[exp_var->i] == '{')
		exp_var->result = expand_bracket_qt(exp_var);
	else if (ft_isalpha(exp_var->str[exp_var->i]) || exp_var->str[exp_var->i] == '_')
		exp_var->result = expand_simple_qt(exp_var);
	else
		exp_var->result = f_strjoin_char(exp_var->result, '$');
	return (exp_var->result);
}

char	*expand_loop(char *str, t_env *env, bool heredoc, char *result)
{
	t_exp_strvars	exp_var;

	exp_var.str = str;
	exp_var.i = 0;
	exp_var.env = env;
	exp_var.result = result;
	exp_var.heredoc = heredoc;
	while (exp_var.str[exp_var.i])
	{
		if (exp_var.str[exp_var.i] == '$')
			exp_var.result = handle_dollar_expansion(&exp_var);
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
