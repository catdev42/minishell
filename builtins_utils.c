/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:25:45 by spitul            #+#    #+#             */
/*   Updated: 2024/10/26 19:42:06 by myakoven         ###   ########.fr       */
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

	a = 0;
	if (ft_strncmp(cmd->argv[0], ECHO, 5) == 0)
		a = echo(cmd);
	else if (ft_strncmp(cmd->argv[0], CD, 3) == 0)
		a = cd(cmd->argv, tool->env, tool);
	else if (ft_strncmp(cmd->argv[0], PWD, 4) == 0)
		a = pwd(cmd);
	else if (ft_strncmp(cmd->argv[0], EXPORT, 7) == 0)
		a = export(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], UNSET, 6) == 0)
		a = unset(cmd, tool);
	// else if (ft_strncmp(cmd->argv[0], ENV, 4) == 0)
	// 	a = env(cmd, tool);
	else if (ft_strncmp(cmd->argv[0], EXIT, 5) == 0)
		a = ft_exit(cmd, tool);
	// tool->exit_code = a;
	record_exit(a, tool);
	return (a);
}

/*UNFINISHED*/
int	append_var(char *key, char *value, char **env, t_tools *tools)
{
	int		i;
	char	*newvar;

	newvar = NULL;
	i = 0;
	i = get_matrix_len(env);
	// while (env[i])
	// 	i++;
	if (i >= tools->env_len - 1)
		copy_env(tools, env); // adds MAXARGS
	newvar = ft_join_one(key, "=", value);
	if (!newvar)
		return (0);
	tools->env[i] = newvar;
	// has to be via tools in case of new env allocation
	return (1);
}

int	replace_or_append_var(char *key, char *value, char **env, t_tools *tools)
{
	int		i;
	char	*temp;
	char	*newvar;
	bool	found;

	found = 0;
	i = 1;
	if (!key)
		return (0);
	if (get_var(env, key))
	{
		found = true;
		temp = env[i];
		newvar = ft_join_one(key, "=", value);
		if (!newvar)
			return (0);
		free(temp);
		env[i] = newvar;
	}
	if (!found)
		if (!append_var(key, value, env, tools))
			return (0);
	return (1);
}
