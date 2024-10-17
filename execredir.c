/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execredir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:34:23 by spitul            #+#    #+#             */
/*   Updated: 2024/10/17 20:47:13 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

//**********************************************************************
// Broken Pipe Error (SIGPIPE): A broken pipe can
//  happen if the command on the reading end of the pipe terminates
// unexpectedly before the writing process finishes. In this case,
// the writing command (the one trying to send data to the pipe)
//  will receive a SIGPIPE signal and often terminate.

// maybe open dir w/ opendir
void	redir_cmd(t_redircmd *rcmd, t_tools *tool)
{
	rcmd->mode = check_file_type(rcmd, rcmd->fd, tool);
	if (rcmd->mode == 0)
		return ; // not sure about this - is a return enough in all cases
	close(rcmd->fd);
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644); // where to close it?
	if (rcmd->fd == -1)
	{
		print_errno_exit(NULL, strerror(errno), NULL, tool);
			// but maybe no exit needed dunno dunno
		// UNFINISHED!
	}
	exec_cmd(rcmd->cmd, tool);
}

/* function forks and sets up and manages pipes*/
pid_t	pipe_fork(int fd, t_cmd *cmd, int pfd, t_tools *tool)
{
	int	pipefd[2];
	int	pid;

	if (pipe(pipefd) == -1)
		print_errno_exit("pipe", NULL, 141, tool);
	pid = fork();
	if (pid == -1)
		print_errno_exit("fork", NULL, 141, tool);
	if (pid == 0)
	{
		close(pipefd[pfd]);
		dup2(pipefd[fd], fd);
		close(pipefd[fd]);
		exec_cmd(cmd, tool);
	}
	else
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (pid);
}

// check if order of running processes correct?
void	pipe_cmd(t_pipecmd *pcmd, t_tools *tools)
{
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	pid1 = pipe_fork(1, pcmd->left, 0, tools);
	pid2 = pipe_fork(0, pcmd->right, 1, tools);
	waitpid(pid1, &status1, 0);
	check_system_fail(status1, tools);
	waitpid(pid2, &status2, 0);
	check_system_fail(status2, tools);
}
