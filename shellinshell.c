/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellinshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:09:25 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/18 23:36:08 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	change_shlvl(t_tools *tool)
{
	char	*var;
	int		shlvl;

	var = get_var(tool->env, "SHLVL");
	shlvl = ft_atoi(var) + 1;
}

// function still needs to be finished
// myakoven Renaming to exec_new_minishell
void	exec_new_minishell(t_tools *tool, t_execcmd *ecmd)
{
	char *shlvl;

	// pid_t	pid;
	if (get_matrix_len(ecmd->argv) != 1)
		print_errno_exit(NULL, "too many arguments", 141, tool);
	shlvl = NULL;
	// pid = fork(); // if not recognized as a builtin forking is not necessary
	// if (pid == -1)
	// 	print_errno_exit(NULL, NULL, NULL, tool); // system error
	// myakoven: this will print mash: strerror(errno) and exit(errno)
	// error_exit(tool, FORKERROR); // i think i dont have the last version
	// if (pid == 0)
	{
		change_shlvl(tool);
		if (execve("./minishell", ecmd->argv, tool->env) == -1)
			print_errno_exit(NULL, NULL, 0, tool);
		// myakoven: this will print mash: strerror(errno) and exit(errno)
		// error_exit(tool, EXECVEERROR);
	}
}