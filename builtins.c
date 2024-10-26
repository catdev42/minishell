/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/26 19:43:55 by myakoven         ###   ########.fr       */
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
	return (0); // exit success
}

// int cd(char *path) {
int	cd(char **argv, char **env, t_tools *tools)
{
	char	*buffer;

	buffer = NULL;
	buffer = safe_calloc(MIDLEN + 1, sizeof(char), tools);
	if (!buffer)
		error_exit(tools, errno);
	if (chdir(argv[1]) < 0)
	{
		print_error("cd", "cannot change directory to %s\n", NULL);
		return (1); // error
	}
	if (!replace_var("PWD", getcwd(buffer, MIDLEN), env)) /* if it errors*/
		return (1);
	free(buffer);
	return (0);
}

int	replace_var(char *key, char *value, char **env)
{
	int		i;
	char	*temp;
	char	*newvar;

	i = 1;
	if (!key)
		return (0);
	while (env[i])
	{
		/*if we find the var value*/
		if (get_var_value(env, key))
		{
			temp = env[i];
			newvar = ft_join_one(key, "=", value);
			if (!newvar)
				return (0);
			free(temp);
			env[i] = newvar;
		}
		i++;
	}
	return (1);
}

// reallocation is dest not provided
char	*ft_join_one(char const *s1, char const *delim, char const *s2)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*fullstr;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(delim) + ft_strlen(s2);
	fullstr = ft_calloc(len + 2, sizeof(char));
	if (!fullstr)
		return (NULL);
	j = 0;
	while (s1[j])
		fullstr[i++] = s1[j++];
	j = 0;
	while (delim[j])
		fullstr[i++] = delim[j++];
	j = 0;
	while (s2[j])
		fullstr[i++] = s2[j++];
	fullstr[i] = 0;
	return (fullstr);
}

// int	export(t_execcmd *cmd, t_tools *tool)
// {
// 	return (1);
// }

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
		/*if we find the var value*/
		if (get_var(tools->env, cmd->argv[i]))
		{
			temp = tools->env[i];
			j = i;
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
	while (str[i] && (ft_isspace(str[i]) || str[i] == '+'))
		i++;
	if (str[i] == '-')
		return (0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (i < ft_strlen(str) - 1)
		return (0);
	return (1);
}

int	ft_exit(t_execcmd *cmd, t_tools *tool)
{
	int	i;

	/*if we have parsed because it was abn exit command*/
	/*We should check args atoll*/
	i = 0;
	if (cmd)
	{
		if (get_matrix_len(cmd->argv) > 2)
			print_error(NULL, "too many arguments", NULL);
		else if (get_matrix_len(cmd->argv) == 2)
		{
			ft_putstr_fd("exit ", 1);
			if (ft_strisnumeric(cmd->argv[1]))
				record_exit(ft_atol(cmd->argv[1]) % 256, tool);
			ft_putstr_fd(tool->exit_string, 1);
		}
		else
			ft_putstr_fd("exit", 1);
	}
	else // if (!tools->line)
		ft_putstr_fd("exit", 1);
	ft_putstr_fd("\n", 1);
	/* If we are here before any tree has been created then we dont do the error thing
	not parsed yes*/
	clean_tools(tool);
	exit(0);
}

int	pwd(t_execcmd *cmd)
{
	char	*cwd;

	cwd = NULL;
	if (get_matrix_len(cmd->argv) > 1)
		ft_putstr_fd("pwd: too many arguments\n", 2);
	cwd = getcwd(NULL, 0); // should we check for malloc error
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
		equalsign[0] = 1;
		i++;
	}
	return (0);
}

int	export(t_execcmd *cmd, t_tools *tool)
{
	int		i;
	char	*equalsign;

	if (!cmd || !cmd->argv[0])
		return (1);
	if (get_matrix_len(cmd->argv) == 1)
		return (print_export(tool->env));
	i = 1;
	while (cmd->argv[i])
	{
		equalsign = ft_strchr(cmd->argv[i], '=');
		if (equalsign && passcheck(cmd->argv[i], (long int)(equalsign
					- &cmd->argv[i][0])))
		{
			equalsign[0] = 0; // nullterm to key
			replace_or_append_var(cmd->argv[i], &equalsign[1], tool->env, tool);
			equalsign[0] = '='; // unnullterm
		}
		else
			break ;
		i++;
	}
	return (0);
}
