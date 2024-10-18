/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execcmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:23:17 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/18 23:23:19 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	*check_cmd_path(char *path, t_execcmd *cmd, t_tools *tools)
{
	char	*cmdpath;
	char	*temp;

	cmdpath = NULL;
	temp = NULL;
	temp = ft_strjoin(path, "/");
	if (!temp)
		print_errno_exit(NULL, NULL, 0, tools); // myakoven system fail
	cmdpath = ft_strjoin(temp, cmd->argv[0]);
	free(temp);
	if (!cmdpath)
		return (NULL); // not failure, just path is not found yet
	if (access(cmdpath, F_OK) == 0)
	{
		if (access(cmdpath, X_OK) != 0) // cannot execute cannot access
		{
			free(cmdpath);
			print_errno_exit(NULL, NULL, 0, tools); // myakoven
		}
		return (cmdpath);
	}
	// printf("msh: permission denied %s\n", cmd->argv[0]);
	free(cmdpath);
	return (NULL);
}

/* is there a point to this being a function? can just stick it directly into */
void	execute_execve(char *cmdpath, t_execcmd *ecmd, t_tools *tool)
{
	execve(cmdpath, ecmd->argv, tool->env);
	free(cmdpath);
	print_errno_exit(NULL, NULL, 0, tool);
}