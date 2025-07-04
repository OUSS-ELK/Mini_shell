#include "../minishell.h"

int	expand_exit_status(t_token **token, bool *space)
{
	char *expanded;

	expanded = ft_itoa(0);
	if (!expanded)
		return (-1);
	add_token(token, expanded, WORD, *space, false);
	*space = false;
	free(expanded);
	return (0);
}

int	expand_bracket_var(t_token **token, char *input, int start, t_env *env, bool *space)
{
	int		len = 0;
	char	*var_name;
	char	*expanded;

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
	add_token(token, expanded, WORD, *space, false);
	*space = false;
	free(expanded);
	return (start + len + 1);
}

int	expand_simple_var(t_token **token, char *input, int start, t_env *env, bool *space)
{
	int		len = 0;
	char	*var_name;
	char	*expanded;

	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start + len] == '_'))
		len++;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (-1);
	expanded = ft_getenv(var_name, env);
	free(var_name);
	if (!expanded)
		expanded = "";
	add_token(token, expanded, WORD, *space, false);
	*space = false;
	return (start + len - 1);
}

int expanding_var(t_token **token, int i, char *input, t_env *env, bool *space, bool heredoc)
{
	int		start;
	int		ret;

	start = heredoc ? i : i + 1;
	if (input[start] == '?')
	{
		ret = expand_exit_status(token, space);
		if (ret == -1)
			return (-1);
		return (start + 1);
	}
	if (input[start] == '{')
	{
		ret = expand_bracket_var(token, input, start + 1, env, space);
		if (ret == -1)
			return (-1);
		return (ret + 1);
	}
	ret = expand_simple_var(token, input, start, env, space);
	if (ret == -1)
		return (-1);
	return (ret + 1);
}

// int expanding_var(t_token **token, int i, char *input, t_env *env, bool *space, bool heredoc)
// {
// 	int		start;
// 	int		len;
// 	char	*var_name;
// 	char	*expanded;

// 	if (!heredoc)
// 		start = i + 1;
// 	else
// 		start = i;
// 	len = 0;
// 	if (input[start] == '?')
// 	{
// 		expanded = ft_itoa(0);												// Should change 0 with exit_status comming from exec
// 		if (!expanded)
// 			return (-1);
// 		add_token(token, expanded, WORD, *space, false);
// 		space = false;
// 		free(expanded);
// 		return (start + 1);
// 	}
// 	if (input[start] == '{')
// 	{
// 		start++; 															// skip '{'
// 		while (input[start + len] && input[start + len] != '}')
// 			len++;
// 		if (input[start + len] != '}')
// 			return (-1); 													// invalid syntax: missing '}'
// 		var_name = ft_substr(input, start, len);
// 		if (!var_name)
// 			return (-1);
// 		expanded = ft_getenv(var_name, env);
// 		free(var_name);
// 		if (!expanded)
// 			expanded = "";
// 		add_token(token, expanded, WORD, *space, false);
// 		*space = false;
// 		return (start + len + 2); 											// +2 for ${ and }
// 	}
// 	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start + len] == '_'))
// 		len++;
// 	// printf("input + len[%c]  | len = %d\n", input[start + len], len);
// 	var_name = ft_substr(input, start, len);
// 	// printf("var_name = %s\n", var_name);
// 	if (!var_name)
// 		return (-1);
// 	expanded = ft_getenv(var_name, env);
// 	// printf("expanded = %s\n", expanded);
// 	free(var_name);
// 	if (!expanded)
// 		expanded = "";
// 	// printf("space [%d]in expand normal\n", *space);
// 	add_token(token, expanded, WORD, *space, false);
// 	if (input[i] && !f_isspace(input[i]))
// 		*space = false;
// 	// else
// 	// 	*space = true;
// 	return (start + len);
// }

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	char	**str;															// str[0] = key & str[1] = value
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

char	*exit_status(char *result, int *i)
{
	char *tmp;

	tmp = ft_itoa(0);
	result = f_strjoin_free(result, tmp);
	free(tmp);
	(*i)++;
	return (result);
}

char	*expand_bracket_qt(char *str, int *i, t_env *env, char *result)
{
	int		start;
	char	*tmp;
	char	*value;

	start = ++(*i);
	while (str[*i] && str[*i] != '}')
		(*i)++;
	if (str[*i] != '}')
		return (result);
	tmp = ft_substr(str, start, *i - start);
	value = ft_getenv(tmp, env);
	result = f_strjoin_free(result, value ? value : "");
	free(tmp);
	(*i)++;
	return (result);
}

char	*expand_simple_qt(char *str, int *i, t_env *env, char *result)
{
	int		start;
	char	*tmp;
	char	*value;

	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	tmp = ft_substr(str, start, *i - start);
	value = ft_getenv(tmp, env);
	result = f_strjoin_free(result, value ? value : "");
	free(tmp);
	return (result);
}

char	*expand_loop(char *str, t_env *env, bool heredoc, char *result)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			if (heredoc)
				i--;
			else if (str[i] == '?')
				result = exit_status(result, &i);
			else if (str[i] == '{')
				result = expand_bracket_qt(str, &i, env, result);
			else if (ft_isalpha(str[i]) || str[i] == '_')
				result = expand_simple_qt(str, &i, env, result);
			else
				result = f_strjoin_char(result, '$');
		}
		else
			result = f_strjoin_char(result, str[i++]);
	}
	return (result);
}

char	*expand_var_str(char *str, t_env *env, bool heredoc)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (expand_loop(str, env, heredoc, result));
}


// char	*expand_var_str(char *str, t_env *env, bool heredoc)
// {
// 	char	*result = ft_strdup("");
// 	int		i = 0, start;
// 	char	*tmp, *value;

// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 		{
// 			i++;
// 			if (heredoc)
// 				i--;
// 			if (str[i] == '?')
// 			{
// 				tmp = ft_itoa(0);											// should replace 0 with exit_status
// 				result = f_strjoin_free(result, tmp);
// 				free(tmp);
// 				i++;
// 			}
// 			else if (str[i] == '{') 										// handle ${VAR}
// 			{
// 				start = ++i;
// 				while (str[i] && str[i] != '}')
// 					i++;
// 				if (str[i] != '}')
// 					return (result);
// 				tmp = ft_substr(str, start, i - start);
// 				value = ft_getenv(tmp, env);
// 				result = f_strjoin_free(result, value ? value : "");
// 				free(tmp);
// 				i++;
// 			}
// 			else if (ft_isalpha(str[i]) || str[i] == '_')
// 			{
// 				start = i;
// 				while (ft_isalnum(str[i]) || str[i] == '_')
// 					i++;
// 				tmp = ft_substr(str, start, i - start);
// 				value = ft_getenv(tmp, env);
// 				result = f_strjoin_free(result, value ? value : "");
// 				free(tmp);
// 			}
// 			else
// 				result = f_strjoin_char(result, '$');
// 		}
// 		else
// 			result = f_strjoin_char(result, str[i++]);
// 	}
// 	return (result);
// }
