/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:18:33 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/19 19:11:57 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	ft_export(char **args, t_env **env_list, t_exec *exec)
{
	int		i;

	g_exit_status = 0;
	if (!env_list || !*env_list)
		return (1);
	if (!args[1])
		return (print_sorted_env(*env_list), 0);
	i = 1;
	while (args[i])
	{
		if (export_syntax_error(args[i]))
		{
			g_exit_status = 1;
			i++;
			continue;
		}
		update_or_add_env(args[i], env_list);
		i++;
	}
	return (g_exit_status);
}

static void	update_or_add_env(char *arg, t_env **env_list)
{
	t_env	*existing;
	char	*key;
	char	*value;
	int		is_append;

	key = NULL;
	value = NULL;
	is_append = 0;
	if (ft_strchr(arg, '='))
	{
		is_append = extract_key_value_project_x(arg, &key, &value);
		if (is_append == -1)
			return ;
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	existing = check_existing_key(*env_list, key); // check dup zbi
	if (existing)
	{
		if (is_append && existing->value)
			handle_append(existing);
		else
			replace_value(existing, value);
		free(key);
	}
	else
		add_new_env(env_list, key, value);
}

static int	export_syntax_error(char *arg)
{
	char	*equal;
	char	*key_end;
	char	*key;

	if (!arg || !*arg)
		return (write(2, "export: not a valid identifier\n", 31), 1);
	equal = ft_strchr(arg, '=');
	if (!equal)
		key_end = arg + ft_strlen(arg);
	else
		key_end = equal;
	if ((ft_strchr(arg, '+')) && ft_strchr(arg, '+') < key_end)
		key_end = ft_strchr(arg, '+');
	key = ft_substr(arg, 0, key_end - arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		write(2, "export: `", 9);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		free(key);
		return (1); // print invalid iden
	}
	free(key);
	return (0);
}

static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i] && name[i] != '=' && name[i] != '+')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	if (name[i] == '+')
	{
		if (!name[i + 1] || name[i + 1] != '=')
			return (0);
	}
	return (1);
}

t_env	*check_existing_key(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}


// == more utils1

// print in declare -x form
void	print_export_env_vars(t_env *head)
{
	while (head)
	{
		if (head && head->key)
		{
			if (head->value == NULL)
				printf("declare -x %s\n", head->key);
			else
			{
				// printf("declare -x %s=", head->key);
				if (head->value[0] == '\0')
					printf("\"\"\n");
				else
					printf("\"%s\"\n", head->value);
			}
		}
		head = head->next;
	}
}

// sort and print env list
void	print_sorted_env(t_env **env)
{
	t_env	*copy;
	t_env	*cur;
	int		size;
	int		i;

	if (!env || !*env)
		return ;
	copy = copy_env_list(*env);
	if (!copy)
		return ;
	size = env_list_size(copy);
	while (size > 0)
	{
		cur = copy;
		while (cur && cur->next)
		{
			swap_env_nodes(cur, cur->next);
			cur = cur->next;
		}
		size--;
	}
	print_export_env_vars(copy);
	free_env(&copy);
}

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = NULL;
	tmp_value = NULL;
	if (ft_strcmp(a->key, b->key) > 0)
	{
		tmp_key = a->key;
		tmp_value = a->value;
		a->key = b->key;
		a->value = b->value;
		b->key = tmp_key;
		b->value = tmp_value;
	}
}

void	update_duplicate_env(t_env *dup, char **key, char **value, char *cmd)
{
	if (!dup || !key || !value)
		return ;
	if (ft_strchr(cmd, '='))
	{
		if (dup->value)
			free(dup->value);
		dup->value = *value;
		*value = NULL;
	}
	else if (*value)
	{
		free(*value);
		*value = NULL;
	}
	if (*key)
	{
		free(*key);
		*key = NULL;
	}
}

// utils2

// error msg
int	print_invalid_identifier(char *cmd)
{
	write(2, "export: `", 9);
	write(2, cmd, ft_strlen(cmd));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}


void	export_assign_helper(char **key, char **value, char *cmd)
{
	*key = ft_strdup(cmd);
	*value = NULL;
}

void	export_append_env_value(t_env *existing, char **key, char **value)
{
	char	*tmp;

	if (!existing || !key || !value || !*value)
		return ;
	tmp = existing->value;
	if (!tmp)
		existing->value = ft_strdup(*value);
	else
	{
		existing->value = ft_strjoin(tmp, *value);
		free(tmp);
	}
	free(*key);
	free(*value);
	*key = NULL;
	*value = NULL;
}

int	parse_export_assignment(char *cmd, char **key, char **value)
{
	int		is_append;
	char	*plus_pos;
	char	*equals_pos;

	is_append = 0;
	plus_pos = ft_strchr(cmd, '+');
	equals_pos = ft_strchr(cmd, '=');
	if (plus_pos && *(plus_pos + 1) == '=')
	{
		is_append = 1;
		*key = ft_substr(cmd, 0, plus_pos - cmd);
		*value = ft_substr(cmd, (plus_pos - cmd) + 2,
				ft_strlen(cmd) - (plus_pos - cmd) - 2);
	}
	else if (equals_pos)
	{
		int key_len = equals_pos - cmd;
		*key = ft_substr(cmd, 0, key_len);
		*value = ft_substr(cmd, key_len + 1, ft_strlen(cmd) - key_len - 1);
	}
	if (!*key)
	{
		if (*value)
			free(*value);
		return (-1);
	}
	return (is_append);
}
