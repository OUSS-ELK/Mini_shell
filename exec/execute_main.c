/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-abde <bel-abde@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 20:39:05 by bel-abde          #+#    #+#             */
/*   Updated: 2025/05/26 17:44:17 by bel-abde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void executor(t_token *tokens, t_exec *exec)
{
	if (!tokens)
		return ;
	exec->env_arr = env_lst_to_arr(exec->env_lst); //todo utils
	// g_sig = 1;
	if (!exec_here_doc(tokens, exec))
		cmd_or_pipe(tokens, exec);
	free_token_list(tokens);
	free_str_arr(&exec->env_arr);
}

