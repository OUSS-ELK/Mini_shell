#include "../minishell.h"

char	*token_list_to_str(t_token *token)
{
	char	*result = ft_strdup("");
	char	*tmp;

	while (token)
	{
        printf("token value = [%s]\n", token->token);
		tmp = result;
		result = ft_strjoin(result, token->token);
		free(tmp);
		token = token->next;
	}
	return result;
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	len1 = ft_strlen(s1);
	size_t	len2 = ft_strlen(s2);
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return ft_strdup(s2);
	else if (!s2)
		return s1;
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (free(s1), free(s2), NULL);
	ft_memcpy(joined, s1, len1);
	ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	free(s1);
	// free(s2);
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
    exp_var.heredoc = false;
    return (expanding_var(&exp_var));
}

void	ft_expand_vars_in_heredoc(char **str, t_env *env)
{
	ssize_t	i = 0;
	int		ret;
	char	*result = NULL;
	t_token	*temp_token = NULL;
	char	temp[2];
	bool	space = false;

	while ((*str)[i])
	{
		while ((*str)[i] && f_isspace((*str)[i]))
		{
			if (!result)
				result = ft_strdup("");
			result = ft_strjoin_free(result, ft_strncpy(&(*str)[i], 1));
			i++;
		}
		if ((*str)[i] == '$')
		{
			printf("found $ \n");
			temp_token = NULL;
			ret = handle_variable_expansion(&temp_token, *str, i, env, &space);
			if (ret == -1)
			{
				free_tokens(&temp_token);
				free(result);
				return ;
			}
			char *expanded = token_list_to_str(temp_token);
			printf("expanded: [%s]\n", expanded);
			result = ft_strjoin_free(result, expanded);
			printf("result: [%s]\n", result);
			free(expanded);
			free_tokens(&temp_token);
			i = ret;
			printf("i = %d\n", i);
		}
		else
		{
			temp[0] = (*str)[i];
			temp[1] = '\0';
			result = ft_strjoin_free(result, temp);
			i++;
			printf("else  i = %d\n", i);
		}
	}
	free(*str);
	*str = result;
}
