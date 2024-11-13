/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:23:17 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/13 15:06:30 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	run_path_exec(t_execcmd *ecmd, t_tools *tool, char *path);

/*Builtins, minishell launch, existing path*/
int	other_execution_type(t_tools *tool, t_execcmd *ecmd)
{
	char	**argv;

	argv = ecmd->argv;
	if (is_builtin(ecmd->argv[0]))
		exit(run_builtin(ecmd, tool));
	if (ft_strncmp(ecmd->argv[0], "minishell", 10) == 0
		|| ft_strncmp(ecmd->argv[0], "./minishell", 12) == 0)
	{
		print_errno_exit(NULL, "This minishell does not handle this!", 1, tool);
	}
	if (!ft_strncmp(argv[0], "/", 1) || !ft_strncmp(argv[0], "./", 2)
		|| !ft_strncmp(argv[0], "../", 3))
	{
		if (file_dir_noexist(ecmd->argv[0], 0) == 2)
			print_errno_exit(ecmd->argv[0], "Is a directory", 126, tool);
		else if (access(ecmd->argv[0], F_OK) == 0)
		{
			if (access(ecmd->argv[0], X_OK) != 0)
				print_errno_exit(NULL, NULL, 126, tool);
			execute_execve(ecmd->argv[0], ecmd, tool);
		}
		else
			exit_with_code(tool, 127);
	}
	return (0);
}

void	run_exec_node(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;

	path = NULL;
	if (!ecmd->argv[0])
		good_exit(tool);
	if (!other_execution_type(tool, ecmd))
	{
		path = get_var_value(tool->env, "PATH");
		if (!path)
			print_errno_exit(NULL, "PATH variable not found", SYSTEMFAIL, tool);
		run_path_exec(ecmd, tool, path);
	}
}

static void	run_path_exec(t_execcmd *ecmd, t_tools *tool, char *path)
{
	char	*cmdpath;
	char	**split_path;
	int		i;

	i = 0;
	cmdpath = NULL;
	split_path = NULL;
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
}

/* is there a point to this being a function? can just stick it directly into */
void	execute_execve(char *cmdpath, t_execcmd *ecmd, t_tools *tool)
{
	execve(cmdpath, ecmd->argv, tool->env);
	free(cmdpath);
	print_errno_exit(NULL, NULL, 0, tool);
}

/*gets mode: check_file_type prints error on error, if fail, exit 1*/
int	run_redir(t_redircmd *rcmd, t_tools *tool)
{
	rcmd->mode = check_file_type(rcmd, rcmd->fd);
	if (rcmd->mode == -1)
		exit_with_code(tool, 1);
	close(rcmd->fd);
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
	if (rcmd->fd == -1)
	{
		print_errno_exit(rcmd->file, strerror(errno), 0, tool);
	}
	handle_node(rcmd->cmd, tool);
	return (1);
}
