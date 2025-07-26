/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:32:21 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 08:47:22 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	alloc_value_part(char **rendu, char *var, int start)
{
	int	val_len;

	if (var[start - 1] != '=')
	{
		rendu[1] = NULL;
		return (0);
	}
	val_len = strlen(var + start);
	rendu[1] = malloc(sizeof(char) * (val_len + 1));
	if (!rendu[1])
	{
		free(rendu[0]);
		free(rendu);
		return (1);
	}
	export_strcpy(rendu[1], var + start);
	return (0);
}

char	**alloc_export_split(char *var)
{
	char	**rendu;
	int		key_len;
	int		start;

	key_len = 0;
	while (var[key_len] && var[key_len] != '+' && var[key_len] != '=')
		key_len++;
	start = get_value_start_index(var, key_len);
	rendu = malloc(sizeof(char *) * 2);
	if (!rendu)
		return (NULL);
	rendu[0] = malloc(sizeof(char) * (key_len + 1));
	if (!rendu[0])
		return (free(rendu), NULL);
	copy_str_n(rendu[0], var, key_len);
	rendu[0][key_len] = '\0';
	if (alloc_value_part(rendu, var, start))
		return (NULL);
	return (rendu);
}

int	get_value_start_index(char *var, int key_len)
{
	if (var[key_len] == '+')
		return (key_len + 2);
	if (var[key_len])
		return (key_len + 1);
	return (key_len);
}

char	**split_export_arg(char *var)
{
	return (alloc_export_split(var));
}

void	export_replace(t_env *var, char ***splitted, char *has_value)
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
