/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:25:45 by spitul            #+#    #+#             */
/*   Updated: 2024/10/22 14:52:10 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*checks if here is a non builtin command in a pipefree tree*/
int	builtin_check_walk(t_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	int					res;

	ecmd = NULL;
	rcmd = NULL;
	res = 0;
	if (cmd)
	{
		if (cmd->type == EXEC)
		{
			ecmd = (t_execcmd *)cmd;
			res = is_builtin(ecmd->argv[0]);
			return (res);
		}
		else
		{
			rcmd = (t_redircmd *)cmd;
			res = builtin_check_walk(rcmd->cmd);
			return (res);
		}
	}
	return (res);
}

int	is_builtin(char *s)
{
	int	a;

	a = 0;
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

	tool->isfork = 1;
	a = 0;
	if (ft_strncmp(cmd->argv[0], ECHO, 5) == 0)
		a = echo(cmd);
	// else if (ft_strncmp(cmd->argv[0], CD, 3) == 0)
	// 	a = cd(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], PWD, 4) == 0)
		a = pwd(cmd);
	// else if (ft_strncmp(cmd->argv[0], EXPORT, 7) == 0)
	// 	a = export(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], UNSET, 6) == 0)
		a = unset(cmd, tool);
	// else if (ft_strncmp(cmd->argv[0], ENV, 4) == 0)
	// 	a = env(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], EXIT, 5) == 0)
		a = ft_exit(cmd, tool);
	return (a);
}
