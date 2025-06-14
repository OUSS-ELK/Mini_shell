#include "../minishell.h"

int expanding_var(t_token **token, int i, char *input, t_env *env, bool space)
{
	int		start;
	int		len;
	char	*var_name;
	char	*expanded;

	start = i + 1;
	len = 0;
	if (input[start] == '?')
	{
		expanded = ft_itoa(0);												// Should change 0 with exit_status comming from exec
		if (!expanded)
			return (-1);
		add_token(token, expanded, WORD, space);
		space = false;
		free(expanded);
		return (start + 1);
	}
	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start + len] == '_' || input[start + len] == '{'))
	{
		if (input[start] == '{')
			start++;
		len++;
	}
	printf("input + len[%c]  | len = %d\n", input[start + len], len);
	var_name = ft_substr(input, start, len);
	printf("var_name = %s\n", var_name);
	if (!var_name)
		return (-1);
	expanded = ft_getenv(var_name, env);
	printf("expanded = %s\n", expanded);
	free(var_name);
	if (!expanded)
		expanded = "";
	add_token(token, expanded, WORD, space);
	space = false;
	return (start + len);
}

t_env	*collect_env(char **env)
{
	t_env	*top;
	t_env	*curr;
	char	**str;							// str[0] = key & str[1] = value
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
