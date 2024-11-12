/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellinshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:09:25 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 01:36:04 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	change_shlvl(t_tools *tool)
{
	char	*var;
	int		shlvl;

	var = get_var_value(tool->env, "SHLVL");
	shlvl = ft_atoi(var) + 1;
	repl_or_app_var("SHLVL", ft_itoa(shlvl), tool->env, tool);
}

// function still needs to be finished
// myakoven Renaming to exec_new_minishell
void	exec_new_minishell(t_tools *tool, t_execcmd *ecmd)
{
	if (get_matrix_len(ecmd->argv) > 1)
		print_errno_exit(NULL, "too many arguments", 141, tool);
	{
		change_shlvl(tool);
		if (execve("./minishell", ecmd->argv, tool->env) == -1)
			print_errno_exit(NULL, NULL, 0, tool);
	}
}
