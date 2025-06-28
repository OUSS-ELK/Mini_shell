/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:18:33 by bel-abde          #+#    #+#             */
/*   Updated: 2025/06/28 15:30:40 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
	Requirements for valid identifier:
		- Not start with: '=', '+' or a digit.
		- Chars before '=' or '+=' must all be: Letters, digits, or underscores.
		- If the user writes VAR+=value then: '+' must be immediately followed by '='.
*/

static int	is_valid_env_var(const char *s, t_exec *flag)
{
	int	i;

	i = 0;
	if (!s || (*s) == '+' || (*s) == '=' || ft_isdigit(*s)) // not valid identifier
		return (0); // print error here (after chaging the current exit status)
	while (s[i] && s[i] != '=' && s[i] != '+')
	{
		if (!ft_isalnum(s[i]) && (s[i] != '_'))
			return (0); // print error here (after chaging the current exit status tp 1)
		i++;
	}
	if (s[i] == '+' && s[i + 1] != '=')
		return (0); // print error here (after chaging the current exit status)
	return (1);
}

// pexort == print export (when we use the cmd export without args)


// addtolst == add a new var using export if it is new
// append == when we use += instead of = (apend new value to the existing one)
// replace == variable exist and += is not used (replace the current value)

void	ft_export(char **av, t_env **env_lst, t_exec *exec)
{
	int		i;

	i = 0;
	exec->exit_status = 0;
	if (!av[1])
		return (pexport(*env_lst)); // print env if no args
	while (av[++i])
	{
		if (!is_valid_env_var(av[i], exec))
			continue ;
		process_export_arg(av[i], env_lst);
	}
}

static void	process_export_arg(char *arg, t_env **env_lst)
{
	t_env	*tmp;
	char	*key;
	char	*value;

	key = get_key(arg);
	value = get_value(arg);
	tmp = *env_lst;
	while (tmp && ft_strcmp(tmp->key, key) != 0)
		tmp = tmp->next;
	if (!tmp)
		add_to_env(env_lst, key, value);
	else if (ft_strnstr(arg, "+=", ft_strlen(arg)))
		append_env_value(tmp, value);
	else
		replace_env_value(tmp, value);
	free(key);
}

void	print_export(t_env **env_lst)
{
	t_env	*current;

	current = *env_lst;
	while (current)
	{
		printf("declare -x %s", current->key);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}
