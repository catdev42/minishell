/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:25:50 by spitul            #+#    #+#             */
/*   Updated: 2024/11/11 19:27:09 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	is_builtin(char *s)
{
	int	a;

	a = 0;
	if (!s)
		return (0);
	if (ft_strncmp(s, ECHO, 5) == 0)
		a = 1;
	else if (ft_strncmp(s, CD, 3) == 0)
		a = 1;
	else if (ft_strncmp(s, PWD, 4) == 0)
		a = 1;
	else if (ft_strncmp(s, EXPORT, 7) == 0)
		a = 1;
	else if (ft_strncmp(s, UNSET, 6) == 0)
		a = 1;
	else if (ft_strncmp(s, ENV, 4) == 0)
		a = 1;
	else if (ft_strncmp(s, EXIT, 5) == 0)
		a = 1;
	return (a);
}

int	run_builtin(t_execcmd *cmd, t_tools *tool)
{
	int	a;

	a = 0;
	record_exit(0,tool);
	if (ft_strncmp(cmd->argv[0], ECHO, 5) == 0)
		a = echo(cmd);
	else if (ft_strncmp(cmd->argv[0], CD, 3) == 0)
		a = cd(cmd->argv, tool->env, tool);
	else if (ft_strncmp(cmd->argv[0], PWD, 4) == 0)
		a = pwd();
	else if (ft_strncmp(cmd->argv[0], EXPORT, 7) == 0)
		a = export(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], UNSET, 6) == 0)
		a = unset(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], ENV, 4) == 0)
		a = env(cmd->argv, tool->env, cmd, tool);
	else if (ft_strncmp(cmd->argv[0], EXIT, 5) == 0)
		a = ft_exit(cmd, tool);
	record_exit(a, tool);
	return (a);
}
