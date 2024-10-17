/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+
		+:	// myakoven: this will print mash: strerror(errno) and exit(errno)
+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:48:13 by spitul            #+#    #+#             */
/*   Updated: 2024/10/15 13:20:31 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

/*forks if there is a pipe or a non builtin command else it
executes without forking
TODO - running minishell inside minishell*/
void	running_msh(t_tools *tools)
{
	pid_t	pid;
	int		status;

	if ((tools->tree->type == PIPE) || (tools->tree->type != PIPE
			&& (builtin_check_walk(tools->tree) == 0)))
	{
		pid = fork();
		if (pid == -1)
			print_errno_exit(NULL, NULL, NULL, tools); // myakoven system fail
		if (pid == 0)
			exec_cmd(tools->tree, tools);
		waitpid(pid, &status, 0);
		check_system_fail(status, tools); // maykoven this also exits
	}
	else
		exec_cmd(tools->tree, tools);
}

char	*check_cmd_in_path(char *path, t_execcmd *cmd, t_tools *tools)
{
	char	*cmdpath;
	char	*temp;

	cmdpath = NULL;
	temp = NULL;
	temp = ft_strjoin(path, "/");
	if (!temp)
		print_errno_exit(NULL, NULL, NULL, tools); // myakoven system fail
	cmdpath = ft_strjoin(temp, cmd->argv[0]);
	free(temp);
	if (!cmdpath)
		return (NULL); // not failure, just path is not found yet
	if (access(cmdpath, F_OK) == 0)
	{
		if (access(cmdpath, X_OK) != 0) // cannot execute cannot access
		{
			free(cmdpath);
			print_errno_exit(NULL, NULL, NULL, tools); // myakoven
		}
		return (cmdpath);
	}
	// printf("msh: permission denied %s\n", cmd->argv[0]);
	free(cmdpath);
	return (NULL);
}

/* is there a point to this being a function? can just stick it directly into */
void	execute_path(char *pathcmd, t_execcmd *ecmd, t_tools *tool)
{
	execve(pathcmd, ecmd->argv, tool->env);
	// free(pathcmd);

	// print_errno_exit(NULL, NULL, 0, tool); // why the third NULL
											// changed null to 0
}

// function still needs to be finished
// myakoven Renaming to exec_new_minishell
void	exec_new_minishell(t_tools *tool, t_execcmd *ecmd)
{
	pid_t	pid;
	char	*shlvl;

	if (get_matrix_len(ecmd->argv) != 1)
		print_errno_exit(NULL, "too many arguments", 141, tool);
	// pid = fork(); // if not recognized as a builtin forking is not necessary
	// if (pid == -1)
	// 	print_errno_exit(NULL, NULL, NULL, tool); // system error
	// myakoven: this will print mash: strerror(errno) and exit(errno)
	// error_exit(tool, FORKERROR); // i think i dont have the last version
	if (pid == 0)
	{
		change_shlvl(tool);
		if (execve("./minishell", ecmd->argv, tool->env) == -1)
			print_errno_exit(NULL, NULL, NULL, tool);
		// myakoven: this will print mash: strerror(errno) and exit(errno)
		// error_exit(tool, EXECVEERROR);
	}
}

/*myakoven... did not get to this one*/
void	check_cmd(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;
	char	*pathcmd;
	char	**split_path;
	int		i;

	i = 0;
	pathcmd = NULL;
	if (is_builtin(ecmd->argv[0]))
	{
		if (run_builtin(ecmd->argv[0]) == -1) // finish heute noch 16.10.
			return ;
	}
	if (ft_strncmp(ecmd->argv[0], "minishell", 9))
	{
		exec_new_minishell(tool, ecmd);
		return ;
	}
	path = get_var(tool->env, "PATH");
	if (!path)
		print_errno_exit(NULL, "PATH variable not found", SYSTEMFAIL, tool);
	// if path var fails, its a system wide issue
	split_path = ft_split(path, ":");
	free(path);
	if (!split_path)
		print_errno_exit(NULL, NULL, NULL, tool);
	while (split_path[i])
	{
		pathcmd = check_cmd_in_path(split_path[i], ecmd->argv[0], tool);
		if (pathcmd != NULL)
		{
			free_tab(split_path);
			execute_path(pathcmd, ecmd, tool->env);
			// execve(pathcmd, ecmd->argv, tool->env); 
			// we can just put execve here since we break, free and error exit on fafilure
			break;
			// if this is terminating and we dont fork how will the path matrix be cleaned? //OK
			//point taken - i revert back to your breaking the loop where we free the path and exit. 
			//previous issue was that it would still check the built in, but we dont do that now!
		}
		i++;
	}
	free(pathcmd);
	print_errno_exit(ecmd->argv[0], "command not found", FORKFAIL, tool); //$?
}

void	exec_cmd(t_cmd *cmd, t_tools *tool)
{
	t_execcmd	*ecmd;
	t_redircmd	*rcmd;
	t_pipecmd	*pcmd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		check_cmd(tool, ecmd);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		redir_cmd(rcmd, tool);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		pipe_cmd(pcmd, tool);
	}
	else
		return (1); // this
}
/*void	_exec_cmd(char *pathcmd, t_execcmd *cmd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (FORK_ERROR);//dunno
	if (pid == 0)
	{
		if (execve(pathcmd, cmd->argv, env) == -1)
		// better an error handling function
			printf("msh: %s: no such file or directory\n", cmd->argv[0]);
	}
	else
		waitpid(pid, NULL, 0);
}*/
