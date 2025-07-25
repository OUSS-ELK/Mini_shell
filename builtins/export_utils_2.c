/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouelkhar <ouelkhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 00:43:01 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/23 15:30:31 by ouelkhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*copy_str_n(char *dest, const char *src, size_t n)
{
	size_t	i;

	if (!dest || !src)
		return (dest);
	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	print_export_vars(t_env *env_list)
{
	while (env_list)
	{
		printf("declare -x %s", env_list->key);
		if (env_list->value)
			printf("=\"%s\"", env_list->value);
		printf("\n");
		env_list = env_list->next;
	}
}

char	*export_strcpy(char *dest, const char *src)
{
	int	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	export_error(const char *str)
{
	write(2, "minishell: export: `", 21);
	write(2, str, strlen(str));
	write(2, "': not a valid identifier\n", 27);
}

int	export_valid_identifier(char *s)
{
	char	*str;

	str = s;
	if (!s || *s == '\0' || *s == '+' || *s == '=' || ft_isdigit(*s))
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
