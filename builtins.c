/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/23 17:27:28 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	echo(t_execcmd *cmd)
{
	int	i;
	int	cmp;

	if (!cmd)
		return (1); // exit fail
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
	replace_var("PWD", getcwd(buffer, MIDLEN), env);
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
		return (1);
	while (env[i])
	{
		/*if we find the var value*/
		if (get_var(env, key))
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

int	unset(t_execcmd *cmd, t_tools *tool)
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
		if (get_var(tool->env, cmd->argv[i]))
		{
			/*put it in temp*/
			temp = cmd->argv[i];
			j = i;
			/*move all the other variables*/
			while (cmd->argv[j++])
			{
				/*
				this might be skipping one variable.
				lets pretend we are at i = 1... you will store it in temp,
				initialize j = 1, check if it exists and
				go to index 2, move stuff from index 3 into 2*/
				cmd->argv[j] = cmd->argv[j + 1];
			}
			/* MAYBE WE NEED TO CHECK THE INCREMENTING OF J */
			free(temp);
		}
		i++;
	}
	return (0);
}

// int	env(t_execcmd *cmd, t_tools *tool)
// {
// 	return (1);
// }

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
