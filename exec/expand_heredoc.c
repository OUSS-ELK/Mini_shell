#include "../minishell.h"
// /* ---------------- UTILS ---------------- */

// static size_t	ms_strlen(const char *s)
// {
// 	size_t i = 0;

// 	if (!s)
// 		return (0);
// 	while (s[i])
// 		i++;
// 	return (i);
// }

// static void	*ms_memcpy(void *dst, const void *src, size_t n)
// {
// 	size_t	i;

// 	if (!dst || !src)
// 		return (dst);
// 	i = 0;
// 	while (i < n)
// 	{
// 		((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
// 		i++;
// 	}
// 	return (dst);
// }


// static char *ms_strdup(const char *s)
// {
// 	size_t	len;
// 	char	*dup;
// 	size_t	i;

// 	if (!s)
// 		return (NULL);
// 	len = 0;
// 	while (s[len])
// 		len++;
// 	dup = malloc(len + 1);
// 	if (!dup)
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		dup[i] = s[i];
// 		i++;
// 	}
// 	dup[i] = '\0';
// 	return (dup);
// }


// int	ms_strcmp(const char *s1, const char *s2)
// {
// 	unsigned char	c1;
// 	unsigned char	c2;

// 	if (s1 == s2)
// 		return (0);
// 	while (*s1 || *s2)
// 	{
// 		c1 = (unsigned char)*s1++;
// 		c2 = (unsigned char)*s2++;
// 		if (c1 != c2)
// 			return (c1 - c2);
// 	}
// 	return (0);
// }

// /* ------------- EXPANSION LOGIC ------------- */

// static ssize_t	ms_find_dollar(const char *str)
// {
// 	size_t i = 0;

// 	if (!str)
// 		return (-1);
// 	while (str[i])
// 	{
// 		if (str[i] == '$')
// 			return (i);
// 		i++;
// 	}
// 	return (-1);
// }

// static int	ms_is_exit_status(const char *str, ssize_t i)
// {
// 	return (str && i >= 0 && str[i] == '$' && str[i + 1] == '?');
// }

// static void	ms_expand_exit_status(char **str)
// {
// 	char	*status_str;
// 	char	*prefix;
// 	char	*suffix;
// 	char	*new_str;
// 	ssize_t	i;

// 	if (!str || !*str)
// 		return;
// 	i = ms_find_dollar(*str);
// 	if (i < 0 || !ms_is_exit_status(*str, i))
// 		return;
// 	status_str = ft_itoa(g_exit_status);
// 	prefix = ft_substr(*str, 0, i);
// 	suffix = ft_substr(*str, i + 2, ms_strlen(*str) - i - 2);
// 	new_str = ms_strjoin_free(prefix, status_str);
// 	new_str = ms_strjoin_free(new_str, suffix);
// 	free(*str);
// 	*str = new_str;
// }

// static char	*ms_expand_var(const char *str, t_env *env, ssize_t i)
// {
// 	char	*var;
// 	size_t	len = 0;
// 	t_env	*curr;

// 	if (!str || i < 0 || str[i] != '$')
// 		return (ms_strdup(""));
// 	while (str[i + 1 + len] && (ft_isalnum(str[i + 1 + len]) || str[i + 1 + len] == '_'))
// 		len++;
// 	var = ft_substr(str, i + 1, len);
// 	curr = env;
// 	while (curr)
// 	{
// 		if (ms_strcmp(var, curr->key) == 0)
// 		{
// 			free(var);
// 			return (curr->value ? ms_strdup(curr->value) : ms_strdup(""));
// 		}
// 		curr = curr->next;
// 	}
// 	free(var);
// 	return (ms_strdup(""));
// }

// static char	*ms_get_suffix(const char *str, ssize_t i)
// {
// 	size_t len = 0;

// 	if (!str || i < 0)
// 		return (ms_strdup(""));
// 	if (str[i + 1] == '?')
// 		return (ft_substr(str, i + 2, ms_strlen(str) - i - 2));
// 	while (str[i + 1 + len] && (ft_isalnum(str[i + 1 + len]) || str[i + 1 + len] == '_'))
// 		len++;
// 	return (ft_substr(str, i + 1 + len, ms_strlen(str) - i - 1 - len));
// }

// /**
//  * Expands all '$VAR' and '$?' inside the given string.
//  * Frees and replaces *str with the expanded result.
//  */
// void	ms_expand_vars_in_str(char **str, t_env *env)
// {
// 	ssize_t i;
// 	char	*value;
// 	char	*joined;
// 	char	*suffix;
// 	char	*old;

// 	if (!str || !*str)
// 		return;
// 	i = ms_find_dollar(*str);
// 	while (i != -1)
// 	{
// 		if (ms_is_exit_status(*str, i))
// 		{
// 			ms_expand_exit_status(str);
// 			i = ms_find_dollar(*str);
// 			continue;
// 		}
// 		value = ms_expand_var(*str, env, i);
// 		joined = ms_strjoin_free(ft_substr(*str, 0, i), value);
// 		suffix = ms_get_suffix(*str, i);
// 		old = *str;
// 		*str = ms_strjoin_free(joined, suffix);
// 		free(old);
// 		free(suffix);
// 		free(value);
// 		i = ms_find_dollar(*str);
// 	}
// }

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	len1 = ft_strlen(s1);
	size_t	len2 = ft_strlen(s2);
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 || !s2)
		return (s1 ? s1 : s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (free(s1), free(s2), NULL);
	ft_memcpy(joined, s1, len1);
	ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	free(s1);
	free(s2);
	return (joined);
}

static int handle_variable_expansion(t_token **token, char *input, int i, t_env *env, bool *space)
{
    t_expndvars	exp_var;

    exp_var.input = input;
    exp_var.i = i;
    exp_var.env = env;
    exp_var.token = token;
    exp_var.space = space;
    exp_var.heredoc = false;  // Set to true for heredoc handling if needed
    return (expanding_var(&exp_var));
}

void	ft_expand_vars_in_str(char **str, t_env *env)
{
    ssize_t	i;
	int		ret;
    char	*result;
    t_token	*token_list;
	char	temp[2];

	result = NULL;
	token_list = NULL;  // This will hold our tokenized output
    i = 0;
	if (!str || !*str)
        return;
    while ((*str)[i])
    {
        // Skip spaces and manage token positions
        while ((*str)[i] && f_isspace((*str)[i]))
        {
            if (!result)
                result = ft_strdup("");  // Start with an empty result if needed
            result = ft_strjoin_free(result, ft_strncpy(&(*str)[i], 1));
            i++;
        }
    	// If we find a '$' (potential variable)
        if ((*str)[i] == '$')
        {
            ret = handle_variable_expansion(&token_list, *str, i, env, NULL);
            if (ret == -1)
                return;  // If there's an error during expansion, return early
            i = ret;  // Continue from where we left off after expansion
        }
        else
        {
            // Copy a regular part of the string to the result
            temp[0] = (*str)[i];
			temp[1] = '\0';
            result = ft_strjoin_free(result, temp);
            i++;
        }
    }
    // Free the old string and set the expanded result
    free(*str);
    *str = result;
}