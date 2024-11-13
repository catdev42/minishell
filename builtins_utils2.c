/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:25:50 by spitul            #+#    #+#             */
/*   Updated: 2024/11/13 15:41:42 by myakoven         ###   ########.fr       */
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
	record_exit(0, tool);
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

int	run_pipeless_builtin_tree(t_cmd *cmd, t_tools *tool)
{
	t_redircmd	*rcmd;

	if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		rcmd->mode = check_file_type(rcmd, rcmd->fd);
		if (rcmd->mode == -1)
		{
			record_exit(1, tool);
			return (0);
		}
		close(rcmd->fd);
		rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
		if (rcmd->fd == -1)
		{
			print_error(rcmd->file, NULL, strerror(errno), NULL);
			return (!record_exit(1, tool));
		}
		run_pipeless_builtin_tree(rcmd->cmd, tool);
	}
	if (cmd->type == EXEC)
		return (run_builtin((t_execcmd *)cmd, tool));
	return (0);
}

int	ft_strisnumeric(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (i < ft_strlen(str))
		return (0);
	return (1);
}
