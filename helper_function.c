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
	// printf("is quote [%c]\n", quote);
	if (quote == '\"' || quote == '\'')
		return (1);
	else
		return 0;
	// return (quote == '\'' || quote == '"');
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
	printf("(inside substring len = %d | start[%d]\n", s_len, start);
	if (start >= s_len || len <= 0)
	{
		printf("(start[%d] >= s_len[%d]|| len <= 0\n", start, s_len);
		return (NULL);
	}
	str = malloc(s_len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		printf("inside while str[%c] | s[%c]\n", str[i], s[start+i]);
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
		printf("token [%s] | type [%d]\n", token->token, token->type);
		token = token->next;
	}
}
