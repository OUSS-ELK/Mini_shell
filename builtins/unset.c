/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:39:15 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/11 15:21:33 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int remove_env_node(t_env **head, t_env *target)
{
    t_env	*prev;
    t_env	*curr;

    if (!head || !*head || !target)
        return (1);
    prev = NULL;
    curr = *head;
    while (curr && curr != target)
    {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) // Target not found
        return (1);
    if (!prev)
        *head = curr->next;
    else
        prev->next = curr->next;
    if (curr->key)
        free(curr->key);
    if (curr->value)
        free(curr->value);
    free(curr);
    return (1);
}

t_env *envnew(char *key, char *value)
{
    t_env *new_node;

    new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
        return (NULL);
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return (new_node);
}

/*
 * Adds a t_env node to the end of the environment list.
 *
 * @param env: Pointer to the head of the environment list
 * @param new: Node to add
 * Side effects: Modifies the list by updating next pointers
 */
void envadd_back(t_env **env, t_env *new)
{
    t_env *current;

    if (!env || !new)
        return;
    if (!*env)
    {
        *env = new;
        return;
    }
    current = *env;
    while (current->next)
        current = current->next;
    current->next = new;
}

/*
 * Initializes a minimal environment when none exists.
 * Creates essential shell environment variables including PWD (current working
 * directory), OLDPWD, SHLVL (shell level), and _ (last command). Used when
 * shell starts with empty environment or as fallback initialization.
 *
 * @param env: Pointer to environment variables linked list (should be empty)
 * @return: 1 if environment was initialized, 0 if environment already exists
 * Side effects: Allocates memory for environment nodes, calls getcwd()
 */
int set_env(t_env **env)
{
    char *cwd;
    char buffer[1024];

    if (!env || *env)
        return (0);

    if (getcwd(buffer, sizeof(buffer)) != NULL)
        cwd = ft_strdup(buffer);
    else
        cwd = ft_strdup("/Users/bel-abde");

    envadd_back(env, envnew(ft_strdup("OLDPWD"), NULL));
    envadd_back(env, envnew(ft_strdup("PWD"), cwd));
    envadd_back(env, envnew(ft_strdup("SHLVL"), ft_strdup("1")));
    envadd_back(env, envnew(ft_strdup("_"), ft_strdup("/usr/bin/env")));
    return (1);
}

static int is_valid_identifier(const char *str)
{
    if (!str || !*str)
        return (0);
    if (!ft_isalpha(*str) && *str != '_')
        return (0);
    str++;
    while (*str)
    {
        if (!ft_isalnum(*str) && *str != '_')
            return (0);
        str++;
    }
    return (1);
}

int ft_unset(char **cmd, t_env **env)
{
    t_env *tmp;
    t_env *to_remove;
    int ret;

    if (!cmd || !*cmd || !env || !*env)
        return (1);
    
    ret = 0;
    cmd++; // Skip "unset" command name
    while (*cmd)
    {
        if (!is_valid_identifier(*cmd))
        {
            // Print error to stderr
            ft_putstr_fd("unset: `", 2);
            ft_putstr_fd(*cmd, 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            ret = 1; // Indicate error
        }
        else if (ft_strcmp(*cmd, "_") != 0)
        {
            tmp = *env;
            while (tmp)
            {
                if (ft_strcmp(tmp->key, *cmd) == 0)
                {
                    to_remove = tmp;
                    tmp = tmp->next;
                    remove_env_node(env, to_remove);
                    break;
                }
                tmp = tmp->next;
            }
        }
        cmd++;
    }
    return (ret);
}

// static int	is_valid(const char *s, t_exec *exec)
// {
// 	int	i;

// 	i = 0;
// 	if (!s || *s == '=' || *s == '+' || ft_isdigit(*s))
// 	{
// 		exec->exit_status = 1;
// 		// print error minishell unset (char *s), "not a valid identifier"
// 		return (0);
// 	}
// 	while (s[i])
// 	{
// 		if (!ft_isalnum(s[i]) && s[i] != '_')
// 		{
// 			exec->exit_status = 1;
// 			// print error minishell unset (char *s), "not a valid identifier"
// 			return (0);
// 		}
// 		i++;
// 	}
// 	return (1);
// }

// void	ft_unset(char **av, t_env **env_lst, t_exec *exec)
// {
// 	t_env	*tmp;
// 	int		i;

// 	if (!env_lst || !av)
// 		return ;
// 	exec->exit_status = 0;
// 	i = 0;
// 	while (av[++i])
// 	{
// 		if (!is_valid(av[i], exec))
// 			continue;
// 		tmp = *env_lst;
// 		while (tmp && ft_strcmp(tmp->key, av[i], ft_strlen(av)) != 0)
// 			tmp = tmp->next;
// 		if (tmp)
// 			free_env_node(env_lst, tmp);
// 	}
// }
