/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:23:17 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/23 19:35:19 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	run_exec_node(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;
	char	*cmdpath;
	char	**split_path;
	int		i;

	i = 0;
	cmdpath = NULL;
	path = NULL;
	if (is_builtin(ecmd->argv[0]))
		exit(run_builtin(ecmd, tool));
	if (ft_strncmp(ecmd->argv[0], "minishell", 9) == 0)
	{
		exec_new_minishell(tool, ecmd);
		return ;
	}
	if (access(ecmd->argv[0], F_OK) == 0)
	{
		if (access(ecmd->argv[0], X_OK) != 0)
			print_errno_exit(NULL, NULL, 0, tool);
		execute_execve(ecmd->argv[0], ecmd, tool);
	}
	path = get_var_value(tool->env, "PATH");
	if (!path)
		print_errno_exit(NULL, "PATH variable not found", SYSTEMFAIL, tool);
	// path is a variable in ENV we do NOT free it
	split_path = ft_split(path, ':');
	if (!split_path)
		print_errno_exit(NULL, NULL, 0, tool);
	while (split_path[i])
	{
		cmdpath = check_cmd_path(split_path[i], ecmd, tool);
		if (cmdpath != NULL)
		{
			ft_freetab(split_path);
			execute_execve(cmdpath, ecmd, tool);
			break ;
		}
		i++;
	}
	free(cmdpath);
	print_errno_exit(ecmd->argv[0], "command not found", 127, tool);
	//$? bash uses 127 as exit code in this case
}

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
		print_errno_exit(NULL, NULL, 0, tools);
	if (access(cmdpath, F_OK) == 0)
	{
		if (access(cmdpath, X_OK) != 0) // cannot execute cannot access
		{
			free(cmdpath);
			print_errno_exit(NULL, NULL, 0, tools);
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
