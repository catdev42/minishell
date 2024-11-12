/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:12:00 by spitul            #+#    #+#             */
/*   Updated: 2024/11/12 21:47:59 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	export(t_execcmd *cmd, t_tools *tool)
{
	int		i;
	char	*eqs;
	int		res;

	res = 0;
	i = 0;
	if (!cmd || !cmd->argv[0])
		return (1);
	if (get_matrix_len(cmd->argv) == 1)
		return (print_export(tool->env));
	while (cmd->argv[++i])
	{
		eqs = ft_strchr(cmd->argv[i], '=');
		if (eqs)
			res = export_check(cmd->argv[i], eqs, tool);
		else
			res = export_check(cmd->argv[i], NULL, tool);
		if (res == 0 || (eqs && ft_strlen(cmd->argv[i]) == 1))
		{
			print_error("export", NULL, "not a valid identifier", NULL);
			tool->exit_code = 1;
		}
	}
	return (tool->exit_code);
}

int	unset(t_execcmd *cmd, t_tools *tools)
{
	int		i;
	int		j;
	char	*temp;

	i = 1;
	if (!cmd)
		return (1);
	while (cmd->argv[i])
	{
		j = -2;
		j = get_var_i(tools->env, cmd->argv[i]);
		if (j > -1)
		{
			temp = tools->env[j];
			while (tools->env[j])
			{
				tools->env[j] = tools->env[j + 1];
				j++;
			}
			free(temp);
		}
		i++;
	}
	return (0);
}

int	print_export(char **env)
{
	int		i;
	char	*equalsign;

	if (!env)
		return (1);
	i = 0;
	while (env[i])
	{
		if (env[i][0] == '_')
		{
			i++;
			continue ;
		}
		equalsign = ft_strchr(env[i], '=');
		equalsign[0] = 0;
		put_var(env[i], &equalsign[1]);
		equalsign[0] = '=';
		i++;
	}
	return (0);
}

void	put_var(char *key, char *value)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("=", 1);
	ft_putstr_fd("\"", 1);
	ft_putstr_fd(value, 1);
	ft_putstr_fd("\"\n", 1);
}

int	export_check(char *arg, char *eqs, t_tools *tool)
{
	int	pass;

	if (eqs)
	{
		pass = passchk(arg, (long int)(eqs - &arg[0]));
		if (pass)
		{
			eqs[0] = 0;
			repl_or_app_var(arg, &eqs[1], tool->env, tool);
			eqs[0] = '=';
			return (1);
		}
		return (0);
	}
	else
	{
		pass = passchk(arg, ft_strlen(arg));
		return (pass);
	}
}
