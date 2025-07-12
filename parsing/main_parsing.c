#include "../minishell.h"

void	else_case(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;

	if (input[st->i] == '$' && input[st->i + 1] == '\0')
	{
		vars.value = "$";
		vars.type = WORD;
		vars.space = st->space;
		vars.quoted = false;
		add_token(token, &vars);
	}
	st->i++;
}

int handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	t_token_vars	vars;
	t_quotevars		qt_var;
	char			*part;
	int				len;

	qt_var.input = input;
	qt_var.start = st->i;
	qt_var.output = &part;
	qt_var.env = env;
	qt_var.heredoc = &st->heredoc;
	len = inside_quote(&qt_var);
	if (len == -1 || !part)
		return (0);
	vars.value = part;
	vars.type = WORD;
	vars.space = st->space;
	vars.quoted = true;
	add_token(token, &vars);
	free(vars.value);
	st->space = f_isspace(input[len]);
	st->i = len;
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}


void	handle_space(char *input, t_lexvars *st)
{
	while (input[st->i] && f_isspace(input[st->i]))
		st->i++;
	st->space = true;
}

int	lexing(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	if (f_isspace(input[st->i]))
		handle_space(input, st);
	else if (is_quote(input[st->i]))
	{
		if (!handle_quote(token, input, env, st))
			return (0);
	}
	else if (is_word_start(input[st->i]))
	{
		if (!handle_word(token, input, st))
			return (0);
	}
	else if (is_operator(input[st->i]))
		st->i = handle_operator(token, input, st->i, st);
	else if (input[st->i + 1] && valid_expand(input[st->i], input[st->i + 1]))
	{
		if (!handle_expansion(token, input, env, st))
			return (0);
	}
	else
		else_case(token, input, st);
	return (1);
}

int lexer_input(t_token **token, char *input, t_env *env)
{
	t_lexvars	st;

	st.i = 0;
	st.space = false;
	st.heredoc = false;
	while (input[st.i])
	{
		if (!lexing(token, input, env, &st))
			return (0);
	}
	if (st.i > 0)
		merge_words(token);
	return (1);
}
