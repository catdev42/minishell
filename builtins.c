/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/24 18:04:26 by myakoven         ###   ########.fr       */
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

/* DRAFT OF ENV: Unchecked */
int	env(char **argv, char **env, t_tools *tools)
{
	pid_t	pid;
	size_t	i;
	char	*equalsign;
	int		status;

	equalsign = NULL;
	if (get_matrix_len(argv) == 1)
		print_tab(env);
	if (get_matrix_len(argv) > 1)
	{
		pid = fork();
		if (pid == 0)
		{
			i = 0;
			while (argv[i]) // assign env variables
			{
				equalsign = ft_strchr(argv[i], '=');
				if (equalsign && passcheck(argv[i], (long int)(equalsign
							- &argv[i][0])))
				{
					equalsign[0] = 0; // nullterm to key
					/*
					replace_or_append_var(argv[i], &equalsign[1], env, tools);
					// TODO*/
					equalsign[0] = '='; // unnullterm
				}
				else
					break ;
				i++;
			}
			if (!argv[i] || !argv[i][0])
			{
				print_tab(tools->env);
				exit(0);
			}
			tools->line = argv[i];
			clean_line(tools->line, ft_strlen(tools->line), tools);
			if (!tools->cleanline)
				print_errno_exit(NULL, NULL, errno, tools);
			if (!parseline(tools->cleanline, tools))
				exit(errno);
			running_msh(tools);
			clean_tools(tools);
			exit(0);
		}
		waitpid(pid, &status, 0);
		check_system_fail(status, tools);
	}
	return (1);
}

int	passcheck(char *start, long int lim)
{
	long int	i;

	i = 0;
	while (i < lim)
	{
		if (ft_isspace(start[i]) || isquote(start[i]))
			return (0);
		i++;
	}
	return (1);
}

/*UNFINISHED*/
int	append_var(char *key, char *value, char **env, t_tools *tools)
{
	int		i;
	char	*newvar;

	newvar = NULL;
	i = 0;
	while (env[i])
		i++;
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
	while (env[i])
	{
		/*if we find the var value*/
		if (get_var_value(env, key))
		{
			found = true;
			temp = env[i];
			newvar = ft_join_one(key, "=", value);
			if (!newvar)
				return (0);
			free(temp);
			env[i] = newvar;
		}
		i++;
	}
	if (!found)
		if (!append_var(key, value, env, tools))
			return (0);
	return (1);
}
int	ft_exit(t_execcmd *cmd, t_tools *tool)
{
	if (get_matrix_len(cmd->argv) > 1)
		print_error(NULL, "too many arguments", NULL);
	// maybe a little nonsensical
	tree_free(tool->tree);
	clean_tools(tool);
	exit(0);
}

int	pwd(t_execcmd *cmd)
{
	char	*cwd;
	size_t	i;

	i = 0;
	cwd = NULL;
	if (get_matrix_len(cmd->argv) > 1)
		ft_putstr_fd("pwd: too many arguments\n", 2);
	cwd = getcwd(NULL, 0); // should we check for malloc error
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

// int	syntax_check_export(char *argv, t_tools *tool)
// {
// 	int	i;

// 	i = 0;
// 	if (argv[0] == '=')
// 	{
// 		print_error("export", "not a valid identifier", argv);
// 		return (1);
// 	}
// 	while (argv[i])
// 	{
// 		if (argv[i] == '=')
// 		{
// 			if (argv[i-1] == ' ')
// 			{
// 				print_error("export", "not a valid identifier", argv);
// 				return (1);
// 			}
// 		}
// 	}

// }
