#include "../minishell.h"

int handle_expansion(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	t_expndvars	exp_var;
	int			new_i;

	exp_var.token = token;
	exp_var.i = st->i;
	exp_var.input = input;
	exp_var.env = env;
	exp_var.space = &st->space;
	exp_var.heredoc = st->heredoc;

	new_i = expanding_var(&exp_var);
	if (new_i == -1)
		return (0);
	st->i = new_i;
	return (1);
}
       
int handle_operator(t_token **token, char *input, int i, t_lexvars *st)
{
	t_oprvars op_vars;

	op_vars.input = input;
	op_vars.i = i;
	op_vars.token = token;
	op_vars.space = st->space;
	op_vars.heredoc = &st->heredoc;

	return (check_operator(&op_vars));
}

int handle_word(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;
	int				start;
	char			*part;

	start = st->i;
	while (input[st->i] && is_word_start(input[st->i]))
		st->i++;
	part = ft_substr(input, start, st->i - start);
	if (!part)
		return (0);
	vars.value = part;
	vars.type = WORD;
	vars.space = st->space;
	vars.quoted = false;
	add_token(token, &vars);
	st->space = f_isspace(input[st->i]);
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

int handle_quote(t_token **token, char *input, t_env *env, t_lexvars *st)
{
	t_token_vars	vars;
	char			*part;
	int				len;

	len = inside_quote(input, st->i, &part, env, &st->heredoc);
	if (len == -1 || !part)
		return (0);
	vars.value = part;
	vars.type = WORD;
	vars.space = st->space;
	vars.quoted = true;
	add_token(token, &vars);
	st->space = f_isspace(input[len]);
	st->i = len;
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

void else_case(t_token **token, char *input, t_lexvars *st)
{
	t_token_vars	vars;

	if (input[st->i + 1] && input[st->i] == '$' && input[st->i + 1] == '\0')
	{
		vars.value = "$";
		vars.type = WORD;
		vars.space = st->space;
		vars.quoted = false;
		add_token(token, &vars);
	}
	st->i++;
}

void handle_space(char *input, t_lexvars *st)
{
	while (input[st->i] && f_isspace(input[st->i]))
		st->i++;
	st->space = true;
}


int lexing(t_token **token, char *input, t_env *env, t_lexvars *st)
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

int parsing_function(t_token **token, char *input, char **env, t_cmd **cmd)
{
	t_env	*envr;

	envr = collect_env(env); 																			// 	collecte environement variables
	if (!envr)
		return (0);																						//	return 0 for error & 1 for success 
	if (!check_quote(input))
	{
		printf("check_quote function\n");
		write_error(2);
		return (free_env(envr), 0);
	}
	if (!lexer_input(token, input, envr))
	{
		// printf("lexer function error\n");
		write_error(3);
		return (free_env(envr), 0);
	}
	// printf("lexer function pass\n");
	*cmd = parse_cmd(token);
	if (!*cmd)
		return (free_env(envr), 0);
	// printf("parse cmd function pass\n");
	free_env(envr);
	return (1);
}
