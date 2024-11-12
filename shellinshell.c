/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellinshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:09:25 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/12 16:44:59 by myakoven         ###   ########.fr       */
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

int	check_mini(t_tools *tools)
{
	t_execcmd	*ecmd;

	if (tools->tree->type == EXEC)
	{
		ecmd = (t_execcmd *)tools->tree;
		if ((ecmd->argv[0]) && ((!ft_strncmp(ecmd->argv[0], "./minishell", 12)
					|| !ft_strncmp(ecmd->argv[0], "minishell", 10))))
		{
			fork_new_minishell(tools);
			return (1);
		}
		return (0);
	}
	return (0);
}

/*reference: heredoc execution*/
/*should return 0 on some sort of failure, */
int	fork_new_minishell(t_tools *tools)
{
	pid_t	pid;

	pid = -1;
	signal_init_sa(tools->sa, SIG_IGN);
	pid = fork();
	if (pid == -1)
		error_exit_main(tools, 1);
	if (pid == 0)
	{
		if (tools->tree->type == EXEC)
			exec_new_minishell(tools, (t_execcmd *)tools->tree);
		printf("Am i still in this child process?\n");
		print_errno_exit(NULL, "This msh does not handle this", 1, tools);
	}
	waitpid(pid, &tools->exit_code, 0);
	check_system_fail(tools->exit_code, tools, 1);
	record_exit(tools->exit_code, tools);
	return (tools->exit_code);
}
