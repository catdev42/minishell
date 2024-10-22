/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/22 20:55:59 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	echo(t_execcmd *cmd)
{
	int	i;
	int	cmp;
	
	if (!cmd)
		return (1);
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
		i ++;
	}
	if (cmp != 0)
		ft_putstr_fd("\n", 1);
	return (0);
}


// int	cd(t_execcmd *cmd, t_tools *tool)
// {
// 	return (1);
// }

int	syntax_check_export(char *argv, t_tools *tool)
{
	int	i;
	
	i = 0;
	if (argv[0] == '=')
	{
		print_error("export", "not a valid identifier", argv);
		return (1);
	}
	while (argv[i])
	{
		if (argv[i] == '=')
		{
			if (argv[i-1] == ' ')
			{
				print_error("export", "not a valid identifier", argv);
				return (1);
			}
		}
	}
		
}

int	export(t_execcmd *cmd, t_tools *tool)
{
	if (get_matrix_len(cmd->argv) > 1)
	
	return (1);
}


int	unset(t_execcmd *cmd, t_tools *tool)
{
	int	i;
	int j;
	char	*temp;

	i = 1;
	if (!cmd)
		return (1);
	while (cmd->argv[i])
	{
		if (get_var(tool->env, cmd->argv[i]))
		{
			temp = cmd->argv[i];
			j = i;
			while (cmd->argv[j ++])
			{
				cmd->argv[j] = cmd->argv[j + 1];
			}
			free (temp);
		}
		i ++;
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
		print_error(NULL, "too many arguments", NULL); //maybe a little nonsensical
	tree_free(tool->tree);
	clean_tools(tool);
	exit (0);
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
