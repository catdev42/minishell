/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/11/11 15:52:14 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	echo(t_execcmd *cmd)
{
	int	i;
	int	cmp;

	if (!cmd)
		return (1); // exit fail
	if (!cmd->argv[1])
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	cmp = ft_strncmp(cmd->argv[1], "-n", 3);
	if (cmp == 0)
		i = 2;
	else
		i = 1;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (cmp != 0)
		ft_putstr_fd("\n", 1);
	return (0);
}

// int cd(char *path) {
int	cd(char **argv, char **env, t_tools *tools)
{
	char	*buffer;

	buffer = NULL;
	buffer = safe_calloc(MIDLEN + 1, sizeof(char), tools);
	if (get_matrix_len(argv) > 2)
	{
		print_error(argv[0], "too many arguments", NULL);
		return (1);
	}
	if (!buffer)
		error_exit_main(tools, errno);
	if (chdir(argv[1]) < 0)
	{
		print_error("cd", strerror(errno), NULL);
		return (1);
	}
	if (!repl_or_app_var("PWD", getcwd(buffer, MIDLEN), env, tools))
		return (1);
	free(buffer);
	return (0);
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

int	ft_exit(t_execcmd *cmd, t_tools *tool)
{
	ft_putstr_fd("exit\n", 1);
	if (cmd)
	{
		if (get_matrix_len(cmd->argv) > 2)
		{
			print_error("exit", "too many arguments", NULL);
			record_exit(1, tool);
		}
		else if (get_matrix_len(cmd->argv) == 2)
		{
			if (ft_strisnumeric(cmd->argv[1]))
				record_exit(ft_atol(cmd->argv[1]), tool);
			else
			{
				print_error("exit", "numeric argument required", NULL);
				record_exit(2, tool);
			}
		}
		else
			record_exit(0, tool);
	}
	else
		record_exit(0, tool);
	clean_tools(tool);
	exit(tool->exit_code);
}

int	pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	if (cwd != NULL)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
		return (0);
	}
	else
		perror("pwd: error retrieving current directory:");
	return (1);
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

int	export(t_execcmd *cmd, t_tools *tool)
{
	int		i;
	int		pass;
	int		otherpass;
	char	*eqs;

	i = 0;
	pass = 0;
	if (!cmd || !cmd->argv[0])
		return (1);
	if (get_matrix_len(cmd->argv) == 1)
		return (print_export(tool->env));
	while (cmd->argv[++i])
	{
		eqs = ft_strchr(cmd->argv[i], '=');
		pass = passchk(cmd->argv[i], (long int)(eqs - &cmd->argv[i][0]));
		otherpass = passchk(cmd->argv[i], ft_strlen(cmd->argv[i]));
		if (eqs && pass)
		{
			eqs[0] = 0;
			repl_or_app_var(cmd->argv[i], &eqs[1], tool->env, tool);
			eqs[0] = '=';
		}
		if ((!eqs && !otherpass) || !pass)
			record_exit(1, tool);
		if ((!pass && !otherpass) || (eqs && ft_strlen(cmd->argv[i]) == 1))
			print_error("export", "not a valid identifier", NULL);
	}
	return (tool->exit_code);
}
