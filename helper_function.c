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
	if (quote == '\'' || quote == '"')
		return (1);
	return (0);
}

int	is_operator(char oper)
{
	return (oper == '|' || oper == '>' || oper == '<');
}


char    *f_substring(char *s, int start, int len)
{
	char    *str;
	int     s_len;
	int     i;

	s_len = f_strlen(s);
	if (start >= s_len || len == 0)
		return (NULL);
	s = s + start;
	s_len = f_strlen(s);
	if (s_len > len)
		s_len = len;
	str = malloc(s_len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[start] && i < len)
		str[i++] = s[start++];
	str[i] = '\0';
	return (str);
}
