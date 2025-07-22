/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:18:33 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/22 08:49:16 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	export_error(const char *str)
{
	write(2, "minishell: export: `", 21);
	write(2, str, strlen(str));
	write(2, "': not a valid identifier\n", 27);
}

static int	export_valid_identifier(char *s)
{
	char	*str = s;

	if (!s || *s == '+' || *s == '=' || ft_isdigit(*s))
		return (export_error(str), 0);

	while (*s && *s != '+' && *s != '=')
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (export_error(str), 0);
		s++;
	}
	if (*s == '+' && *(s + 1) != '=')
		return (export_error(str), 0);
	return (1);
}

static char	**export_alloc_rendu(char *var)
{
	char	**rendu;
	int		key_len = 0;
	int		val_len;
	int		start;

	while (var[key_len] && var[key_len] != '+' && var[key_len] != '=')
		key_len++;

	start = (var[key_len] == '+') ? key_len + 2 : (var[key_len] ? key_len + 1 : key_len);

	rendu = malloc(sizeof(char *) * 2);
	if (!rendu)
		return (NULL);

	rendu[0] = malloc(sizeof(char) * (key_len + 1));
	if (!rendu[0])
		return (free(rendu), NULL);
	strncpy(rendu[0], var, key_len);
	rendu[0][key_len] = '\0';

	val_len = strlen(var + start);
	rendu[1] = malloc(sizeof(char) * (val_len + 1));
	if (!rendu[1])
		return (free(rendu[0]), free(rendu), NULL);
	strcpy(rendu[1], var + start);

	return (rendu);
}

static char	**export_split_var(char *var)
{
	return export_alloc_rendu(var);
}

static void	export_replace(t_env *var, char ***splitted, char *has_value)
{
	if (has_value)
	{
		free(var->value);
		var->value = (*splitted)[1];
	}
	else
		free((*splitted)[1]);
	free((*splitted)[0]);
	free(*splitted);
	*splitted = NULL;
}

int	export_add_node(t_env **env_lst, char **splitted)
{
	t_env	*new;
	t_env	*head;

	if (!env_lst || !splitted)
		return (0);
	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = splitted[0];
	new->value = splitted[1];
	new->next = NULL;
	if (!(*env_lst))
		*env_lst = new;
	else
	{
		head = *env_lst;
		while (head->next)
			head = head->next;
		head->next = new;
	}
	return (1);
}

static void	export_add_to_env(t_env **env, char ***splitted, char *has_value)
{
	if (!has_value)
	{
		free((*splitted)[1]);
		(*splitted)[1] = NULL;
	}
	export_add_node(env, *splitted);
	free(*splitted);
	*splitted = NULL;
}

static void	export_append(t_env *var, char ***splitted)
{
	char	*new_val;

	new_val = ft_strjoin(var->value, (*splitted)[1]);
	free(var->value);
	var->value = new_val;

	free((*splitted)[0]);
	free((*splitted)[1]);
	free(*splitted);
	*splitted = NULL;
}

void	export_print(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

void	ft_export(char **av, t_exec *x)
{
	t_env	*var;
	char	**splitted;
	int		i;

	if (!av[1])
		return (export_print(x->env_lst));
	i = 0;
	while (av[++i])
	{
		if (!export_valid_identifier(av[i]))
			continue;
		splitted = export_split_var(av[i]);
		if (!splitted)
			continue;
		var = x->env_lst;
		while (var && ft_strcmp(var->key, splitted[0]) != 0)
			var = var->next;
		if (!var)
			export_add_to_env(&(x->env_lst), &splitted, ft_strchr(av[i], '='));
		else if (ft_strchr(av[i], '=') && *(ft_strchr(av[i], '=') - 1) == '+')
			export_append(var, &splitted);
		else
			export_replace(var, &splitted, ft_strchr(av[i], '='));
	}
}

// int	check_input(char *name, int *flag)
// {
// 	int	i = 1;

// 	while (name[i] && name[i] != '=')
// 	{
// 		if ((!(name[i] >= '_' && name[i] <= 'z') && !(name[i] >= 'A'
// 				&& name[i] <= 'Z') && !(name[i] >= '0' && name[i] <= '9')))
// 		{
// 			if (name[i] == '+' && name[i + 1] == '=')
// 				break ;
// 			*flag = 0;
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (!(*flag))
// 	{
// 		ft_putstr_fd("bash: export: `", 2);
// 		ft_putstr_fd(name, 2);
// 		ft_putstr_fd("': not a valid identifier\n", 2);
// 		g_exit_status = 1;
// 		return (0);
// 	}
// 	return (1);
// }


// int	is_valid(char *name)
// {
// 	int	flag = 1;

// 	if (!(name[0] >= '_' && name[0] <= 'z') && !(name[0] >= 'A' && name[0] <= 'Z'))
// 		flag = 0;
// 	if (name[0] == '-')
// 	{
// 		ft_putstr_fd("bash: export: `", 2);
// 		ft_putstr_fd(name, 2);
// 		ft_putstr_fd("': invalid option\n", 2);
// 		g_exit_status = 1;
// 		return (0);
// 	}
// 	if (!check_input(name, &flag))
// 		return (0);
// 	return (1);
// }



// void	get_key_value_split(char *name, char **key, char **value, bool *append)
// {
// 	char	*eq;

// 	*append = false;
// 	eq = ft_strchr(name, '=');
// 	if (!eq)
// 	{
// 		*key = ft_strdup(name);
// 		*value = NULL;
// 		return ;
// 	}
// 	if (eq > name && *(eq - 1) == '+')
// 	{
// 		*append = true;
// 		*key = ft_substr(name, 0, (eq - name) - 1);
// 	}
// 	else
// 		*key = ft_substr(name, 0, eq - name);
// 	*value = ft_strdup(eq + 1);
// }


// void	ft_export(char *name, t_env **head)
// {
// 	t_env	*curr;
// 	char	*key;
// 	char	*value;
// 	bool	append;
// 	int		found;

// 	if (!is_valid(name) || !head)
// 		return ;

// 	get_key_value_split(name, &key, &value, &append);
// 	if (!key)
// 		return ;

// 	curr = *head;
// 	found = 0;
// 	while (curr)
// 	{
// 		if (ft_strcmp(curr->key, key) == 0)
// 		{
// 			found = 1;
// 			if (value)
// 			{
// 				if (append && curr->value)
// 				{
// 					char *temp = curr->value;
// 					curr->value = ft_strjoin(curr->value, value);
// 					free(temp);
// 				}
// 				else
// 				{
// 					free(curr->value);
// 					curr->value = ft_strdup(value);
// 				}
// 			}
// 			break ;
// 		}
// 		curr = curr->next;
// 	}
// 	if (!found)
// 	{
// 		t_env *new = malloc(sizeof(t_env));
// 		new->key = ft_strdup(key);
// 		new->value = value ? ft_strdup(value) : NULL;
// 		new->next = NULL;
// 		export_lstadd_back(head, new);
// 	}
// 	free(key);
// 	if (value)
// 		free(value);
// }
///////////////////////////////////////////////////////////////////////////////////////////////////

// t_env	*export_check_dup(char *key, t_env *env)
// {
// 	t_env	*tmp = env;

// 	printf("[EXPORT DEBUG] Checking for duplicate key: %s\n", key);
// 	while (tmp)
// 	{
// 		printf("  [EXPORT DEBUG] Comparing with: %s\n", tmp->key);
// 		if (ft_strcmp(tmp->key, key) == 0)
// 		{
// 			printf("  [EXPORT DEBUG] Duplicate key found: %s\n", key);
// 			return (tmp);
// 		}
// 		tmp = tmp->next;
// 	}
// 	printf("  [EXPORT DEBUG] No duplicate key found\n");
// 	return (NULL);
// }

// void export_alloc_key_value(char *cmd, char **key, char **value, t_env **env)
// {
//     t_env *dup_key = NULL;
//     t_env *new = NULL;
//     int is_append = 0;

//     printf("[EXPORT DEBUG] Allocating from cmd: '%s'\n", cmd);
//     if (ft_strchr(cmd, '='))
//     {
//         is_append = export_handle_plus(cmd, key, value);
//         if (is_append == -1)
//         {
//             printf("[EXPORT DEBUG] export_handle_plus failed for: %s\n", cmd);
//             return;
//         }
//     }
//     else
//     {
//         export_helper(key, value, cmd);
//     }

//     if (!key || !*key)
//     {
//         printf("[EXPORT DEBUG] Key is NULL — skipping\n");
//         return;
//     }

//     printf("[EXPORT DEBUG] Parsed key: '%s', value: '%s'\n", *key, *value ? *value : "(null)");

//     dup_key = export_check_dup(*key, *env);
//     if (dup_key)
//     {
//         if (is_append && dup_key->value)
//         {
//             printf("[EXPORT DEBUG] Appending to existing key: %s\n", dup_key->key);
//             export_handle_append(dup_key, key, value);
//         }
//         else
//         {
//             printf("[EXPORT DEBUG] Updating existing key: %s\n", dup_key->key);
//             export_alloc_dup(dup_key, key, value, cmd);
//         }
//         // Free temporary strings after handling duplicates
//         free(*key);
//         *key = NULL;
//         if (*value)
//         {
//             free(*value);
//             *value = NULL;
//         }
//     }
//     else
//     {
//         new = export_lstnew(*key, *value);
//         if (!new)
//         {
//             printf("[EXPORT DEBUG] export_lstnew failed for key: %s\n", *key);
//             free(*key);
//             if (*value) free(*value);
//             return;
//         }
//         export_lstadd_back(env, new);
//         printf("[EXPORT DEBUG] New env node added: key='%s', value='%s'\n", *key, *value ? *value : "(null)");
//         // Don't free key/value here - they're now owned by the env list
//     }
// }

// static int	export_valid_identifier(char *str)
// {
// 	int	i = 0;

// 	if (!str || !*str)
// 	{
// 		printf("[EXPORT DEBUG] Identifier is NULL or empty\n");
// 		return (0);
// 	}
// 	if (!ft_isalpha(str[0]) && str[0] != '_')
// 	{
// 		printf("[EXPORT DEBUG] Invalid start of identifier: %c\n", str[0]);
// 		return (0);
// 	}
// 	while (str[i] && str[i] != '=' && str[i] != '+')
// 	{
// 		if (!ft_isalnum(str[i]) && str[i] != '_')
// 		{
// 			printf("[EXPORT DEBUG] Invalid char in identifier: %c\n", str[i]);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	if (str[i] == '+' && (!str[i + 1] || str[i + 1] != '='))
// 	{
// 		printf("[EXPORT DEBUG] '+' not followed by '=' in identifier\n");
// 		return (0);
// 	}
// 	return (1);
// }

// int	export_check_error(char *cmd)
// {
// 	char	*equals_pos;
// 	char	*name_end;
// 	char	*tmp;

// 	printf("[EXPORT DEBUG] Checking identifier: '%s'\n", cmd);

// 	if (!cmd || !*cmd)
// 		return (write(2, "export: `': not a valid identifier\n", 35), 1);
// 	if (*cmd == '=')
// 		return (export_print_invalid_identifier("="));

// 	equals_pos = ft_strchr(cmd, '=');
// 	name_end = equals_pos ? equals_pos : cmd + ft_strlen(cmd);
// 	if (ft_strchr(cmd, '+'))
// 		name_end = ft_strchr(cmd, '+');

// 	tmp = ft_substr(cmd, 0, name_end - cmd);
// 	if (!tmp)
// 	{
// 		printf("[EXPORT DEBUG] ft_substr failed\n");
// 		return (1);
// 	}

// 	if (!export_valid_identifier(tmp))
// 	{
// 		printf("[EXPORT DEBUG] Invalid identifier: '%s'\n", tmp);
// 		free(tmp);
// 		return (export_print_invalid_identifier(cmd));
// 	}

// 	free(tmp);
// 	return (0);
// }

// void print_env_list(t_env *env)
// {
//     t_env *tmp = env;
//     int count = 0;
//     printf("[DEBUG] Current env list:\n");
//     while (tmp && count < 100) // limit to 100 nodes
//     {
//         printf("  node %p: key='%s', value='%s'\n",
//             (void*)tmp,
//             tmp->key ? tmp->key : "(null)",
//             tmp->value ? tmp->value : "(null)");
//         tmp = tmp->next;
//         count++;
//     }
//     if (tmp)
//         printf("[DEBUG] ... list truncated after 100 nodes\n");
// }


// int ft_export(char **cmd, t_env **env)
// {
//     char *key = NULL;
//     char *value = NULL;
//     int ret = 0;

//     printf("[EXPORT DEBUG] ft_export() called\n");
//     if (!env)
//     {
//         printf("[EXPORT DEBUG] env is NULL\n");
//         return (1);
//     }

//     if (!cmd[1])
//     {
//         printf("[EXPORT DEBUG] No arguments — calling export_print_sorted_env\n");
//         export_print_sorted_env(env);
//     }
//     else
//     {
//         cmd++;
//         while (*cmd)
//         {
//             printf("[EXPORT DEBUG] Processing arg: '%s'\n", *cmd);
//             if (export_check_error(*cmd) != 0)
//             {
//                 printf("[EXPORT DEBUG] Identifier error for: '%s'\n", *cmd);
//                 ret = 1;
//             }
//             else
//             {
//                 export_alloc_key_value(*cmd, &key, &value, env);
//                 // Print env list after adding/updating a variable
//                 print_env_list(*env);
//             }
//             cmd++;
//         }
//     }

//     printf("[EXPORT DEBUG] ft_export() finished with return: %d\n", ret);
//     return (ret);
// }


// t_env	*export_check_dup(char *key, t_env *env)
// {
// 	t_env	*tmp = env;

// 	while (tmp)
// 	{
// 		if (ft_strcmp(tmp->key, key) == 0)
// 			return (tmp);
// 		tmp = tmp->next;
// 	}
// 	return (NULL);
// }

// void	export_alloc_key_value(char *cmd, char **key, char **value, t_env **env)
// {
// 	t_env	*dup_key = NULL;
// 	int		is_append = 0;

// 	if (ft_strchr(cmd, '='))
// 	{
// 		is_append = export_handle_plus(cmd, key, value);
// 		if (is_append == -1)
// 			return ;
// 	}
// 	else
// 		export_helper(key, value, cmd);
// 	dup_key = export_check_dup(*key, *env);
// 	if (dup_key)
// 	{
// 		if (is_append && dup_key->value)
// 			export_handle_append(dup_key, key, value);
// 		else
// 			export_alloc_dup(dup_key, key, value, cmd);
// 	}
// 	else
// 	{
// 		t_env *new = export_lstnew(*key, *value);
// 		if (new)
// 			export_lstadd_back(env, new);
// 	}
// }

// static int	export_valid_identifier(char *str)
// {
// 	int	i = 0;

// 	if (!str || !*str)
// 		return (0);
// 	if (!ft_isalpha(str[0]) && str[0] != '_')
// 		return (0);
// 	while (str[i] && str[i] != '=' && str[i] != '+')
// 	{
// 		if (!ft_isalnum(str[i]) && str[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	if (str[i] == '+' && (!str[i + 1] || str[i + 1] != '='))
// 		return (0);
// 	return (1);
// }

// int	export_check_error(char *cmd)
// {
// 	char	*equals_pos;
// 	char	*name_end;
// 	char	*tmp;

// 	if (!cmd || !*cmd)
// 		return (write(2, "export: `': not a valid identifier\n", 35), 1);
// 	if (*cmd == '=')
// 		return (export_print_invalid_identifier("="));
// 	equals_pos = ft_strchr(cmd, '=');
// 	name_end = equals_pos ? equals_pos : cmd + ft_strlen(cmd);
// 	if (ft_strchr(cmd, '+'))
// 		name_end = ft_strchr(cmd, '+');
// 	tmp = ft_substr(cmd, 0, name_end - cmd);
// 	if (!tmp)
// 		return (1);
// 	if (!export_valid_identifier(tmp))
// 	{
// 		free(tmp);
// 		return (export_print_invalid_identifier(cmd));
// 	}
// 	free(tmp);
// 	return (0);
// }

// int	ft_export(char **cmd, t_env **env)
// {
// 	char	*key = NULL;
// 	char	*value = NULL;
// 	int		ret = 0;

// 	if (!env)
// 		return (1);
// 	if (!cmd[1])
// 		export_print_sorted_env(env);
// 	else
// 	{
// 		cmd++;
// 		while (*cmd)
// 		{
// 			if (export_check_error(*cmd) != 0)
// 				ret = 1;
// 			else
// 				export_alloc_key_value(*cmd, &key, &value, env);
// 			cmd++;
// 		}
// 	}
// 	return (ret);
// }
