#include "minishell.h"

void	write_error(int	n)
{
	if (n == 1)
		write(2, "Error in Parsing\n", 17);
	else
		write(2, "Error\n", 6);
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

int f_strlen(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
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

char    *f_substring(char *s, int start, int len)
{
	char    *str;
	int     s_len;
	int     i;

	s_len = f_strlen(s);
	if (start >= s_len || len < 0)
	{
		return (NULL);
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	print_tokens(t_token *token)
{
	while (token)
	{
		printf(BLUE"token [%s] | type [%d] | space[%d]\n" RESET, token->token, token->type, token->space);
		token = token->next;
	}
}

char	*f_strdup(char *s)
{
	int	i = 0;

	while (s[i])
		i++;
	char	*str = malloc(i + 1);
	if (!str)
		return NULL;
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*f_strjoin(char *s1, char *s2)
{
	char	*joined;
	int		len;
	int		i = 0;
	int		x = 0;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (f_strdup(s2));
	if (!s2)
		return (f_strdup(s1));
	len = f_strlen(s1) + f_strlen(s2);
	joined = malloc(len + 1);
	if (!joined)
		return (NULL);
	joined[len] = '\0';
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	while (s2[i])
		joined[i++] = s2[x++];
	free(s1);
	// free(s2);
	return (joined);
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
