#include "../minishell.h"

void	write_error(int	n)
{
	if (n == 1)
		write(2, "Parsing Error\n", 14);
	else if (n == 2)
		write(2, "Lexer Error\n", 12);
	else if (n == 3)
		write(2, "Quote Error\n", 12);
	else if (n == 4)
		write(2, "syntax error: missing filename after redirection\n", 49);
	else if (n == 5)
		write(1, "Syntax error empty command before pipe\n", 39);
	else if (n == 6)
		write(1, "Syntax error empty command after pipe\n", 38);
	else if (n == 7)
		write(1, "Syntax error ambiguous redirect\n", 32);
	else
		write(2, "Error\n", 6);
}

void	cleanup(t_token *token, t_cmd *cmd, char *input)
{
	if (token)
		free_tokens(token);
	if (cmd)
		free_cmd(cmd);
	if (input)
		free(input);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
			free_array(cmd->args);
		if (cmd->redir)
			free_redir(cmd->redir);
		free(cmd);
		cmd = tmp;
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_array(char **str)
{
	char	**tmp;

	if (!str)
		return ;
	tmp = str;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(str);
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

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

t_token	*get_last_token(t_token *token)
{
	if (!token)
		return (NULL);

	while (token->next)
		token = token->next;
	return (token);
}

int	all_space(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (1);
	while (input[i])
	{
		if (!f_isspace(input[i]) && input[i] != '$')
			return (0);
		i++;
	}
	return (1);
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
	return (!f_isspace(c) && !is_operator(c) && !is_quote(c) && c != '$');
}

int	is_alpha(char input)
{
	return ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'));
}

int	valid_expand(char input, char next)
{
	return (input == '$' && (ft_isalnum(next) || next == '{' || next == '_' || next == '?' || next == '$'));
}

void	print_env(t_env *env)
{
	while (env)
	{
		printf(BLUE"KEY [%s] | VALUE [%s] | \n" RESET, env->key, env->value);
		env = env->next;
	}
}

void	print_tokens(t_token *token)
{
	while (token)
	{
		printf(BLUE"TOKEN [%s] | TYPE [%d] | space[%d] | quote[%d]\n" RESET, token->token, token->type, token->space, token->quote);
		token = token->next;
	}
}

void    ll(void)
{
    system("leaks -q minishell");
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

void	print_array(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		printf("arr[%s]\n", arr[i]);
		i++;
	}
}

void	print_cmds(t_cmd *cmd)
{
	int i = 0;
	while (cmd)
	{
		printf(RED"==== CMD ====\n"RESET);
		if (cmd->args)
		{
			printf(YELLOW"Args:\n"RESET);
			while (cmd->args[i])
				printf(BLUE"  [%s]\n"RESET, cmd->args[i++]);
			i = 0;
		}

		t_redir *r = cmd->redir;
		while (r)
		{
			printf(GREEN"Redir: type[%d], filename [%s] | quote[%d]\n"RESET, r->type, r->filename, r->quoted);
			r = r->next;
		}
		cmd = cmd->next;
	}
}

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
	free(part);
	st->space = f_isspace(input[st->i]);
	if (st->heredoc)
		st->heredoc = false;
	return (1);
}

char	*extract_quoted_content(char *input, int start, int *end)
{
	int		i;
	char	*str;
	char	quote;

	quote = input[start];
	i = start + 1;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] != quote)
		return (NULL);
	str = ft_substr(input, start + 1, i - start - 1);
	if (!str)
		return (NULL);
	*end = i;
	return (str);
}

char	*handle_quote_expansion(char *str, char quote, t_env *env, bool heredoc)
{
	char	*expand;

	if (heredoc || quote == '\'')
		expand = ft_strdup(str);
	else
		expand = expand_var_str(str, env, heredoc);
	return (expand);
}

int inside_quote(t_quotevars *qt_var)
{
	char	*str;
	char	*expand;
	int		end;
	char	quote;

	quote = qt_var->input[qt_var->start];
	str = extract_quoted_content(qt_var->input, qt_var->start, &end);
	if (!str)
		return (-1);
	expand = handle_quote_expansion(str, quote, qt_var->env, *(qt_var->heredoc));
	free(str);
	if (!expand)
		return (-1);
	*(qt_var->output) = expand;
	return (end + 1);
}

int	is_only_space(char *filename)
{
	int	i;

	i = 0;
	while (filename[i])
	{
		if (!f_isspace(filename[i]))
			return (1);
		i++;
	}
	return (0);
}

int is_oper(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == HEREDOC);
}

t_cmd	*alloc_new_cmd(void)
{
	t_cmd *new;

	new = ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->redir = NULL;
	new->next = NULL;
	return (new);
}

char	**handl_word(char **args, char *new_arg)
{
	int		len;
	char	**new_args;
	int 	i;

	len = 0;
	i = 0;
	if (args)
		while (args[len])
			len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);
	while ( i < len)
	{
		new_args[i] = ft_strdup(args[i]);
		if (!new_args[i])
			return (free_array(new_args), NULL);
		i++;
	}
	new_args[len] = ft_strdup(new_arg);
	if (!new_args[len])
		return (free(new_args), NULL);
	new_args[len + 1] = NULL;
	free_array(args);
	return (new_args);
}

int is_valid_redir_filename(t_token *op_token)
{
	char *filename;

	filename = op_token->next->token;
	if (!filename || filename[0] == '\0')
	{
		if (op_token->type != HEREDOC)
		{
			write_error(7);
			return (0);
		}
	}
	return (1);
}

void	add_redirection(t_redir **redir_list, t_redir *new_redir)
{
	t_redir *temp;

	if (!*redir_list)
	{
		*redir_list = new_redir;
		return ;
	}
	temp = *redir_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_redir;
}

int create_and_add_redir(t_cmd *cmd, t_token *redir_token)
{
	t_redir *new_redir;
	char *filename = redir_token->next->token;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (0);
	new_redir->filename = ft_strdup(filename);
	if (!new_redir->filename || !is_only_space(new_redir->filename))
	{
		free(new_redir);
		return (0);
	}
	new_redir->type = redir_token->type;
	new_redir->quoted = redir_token->next->quote;
	new_redir->next = NULL;
	add_redirection(&cmd->redir, new_redir);
	return (1);
}

int handle_redirection(t_cmd *cmd, t_token **curr_token)
{
	if (!(*curr_token)->next || (*curr_token)->next->type != WORD)
	{
		write_error(4);
		return (0);
	}
	if (!is_valid_redir_filename(*curr_token))
		return (0);
	if (!create_and_add_redir(cmd, *curr_token))
		return (0);
	*curr_token = (*curr_token)->next->next;
	return (1);
}
