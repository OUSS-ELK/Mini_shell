/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 07:00:04 by bel-abde          #+#    #+#             */
/*   Updated: 2025/07/26 07:00:20 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_tilde(char *input, t_env *env)
{
	char	*home;
	char	*expanded;

	if (!input || input[0] != '~')
		return (ft_strdup(input));
	home = ft_getenv_value(env, "HOME");
	if (!home)
		return (NULL);
	expanded = ft_strjoin(home, input + 1);
	return (expanded);
}
