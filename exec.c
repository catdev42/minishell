/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:23:33 by spitul          #+#    #+#             */
/*   Updated: 2024/10/19 17:33:20 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*forks if there is a pipe or a non builtin command else it
executes without forking
TODO - running minishell inside minishell*/
int	running_msh(t_tools *tools)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!tools->tree)
		return (0);
	if ((tools->tree->type == PIPE) || (tools->tree->type != PIPE
			&& (builtin_check_walk(tools->tree) == 0)))
	{
		// tools->isfork = 1;
		pid = fork();
		if (pid == -1)
			print_errno_exit(NULL, NULL, 0, tools); // myakoven system fail
		if (pid == 0)
		{
			tools->sa.sa_handler = SIG_DFL; //NEW TODO
			handle_node(tools->tree, tools);
		}
		waitpid(pid, &status, 0);
		check_system_fail(status, tools); // maykoven this also exits
	}
	else
	{
		run_pipeless_builtin_tree(tools->tree, tools);
	}
	return (1);
}

void	handle_node(t_cmd *cmd, t_tools *tool)
{
	t_execcmd	*ecmd;
	t_redircmd	*rcmd;
	t_pipecmd	*pcmd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		run_exec_node(tool, ecmd);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		run_redir(rcmd, tool);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		run_pipe(pcmd, tool);
	}
	else
		print_errno_exit(NULL, "unknown error", 141, tool);
	/*
	This is an error exit, because there is no else,
		this just catches any unknown errors... if we dont terminate all the stuff in exec node we exit error
	*/
}

/* function forks and sets up and manages pipes*/

void	run_pipe(t_pipecmd *pcmd, t_tools *tools)
{
	int		pipefd[2];
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		print_errno_exit(NULL, NULL, 0, tools);
	pid1 = fork();
	if (pid1 == -1)
		print_errno_exit(NULL, NULL, 0, tools);
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		handle_node(pcmd->left, tools); // terminating
	}
	close(pipefd[1]);
	// if parent keeps the writing end open the pipe
	// will not be considered closed by the reading process
	pid2 = fork();
	if (pid2 == -1)
		print_errno_exit(NULL, NULL, 0, tools);
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		handle_node(pcmd->right, tools); // terminating
	}
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(pid1, &status1, 0);
	check_system_fail(status1, tools);
	waitpid(pid2, &status2, 0);
	check_system_fail(status2, tools);
}

// maybe open dir w/ opendir
/*MYAKOVEN: I think this function only need to get the mode,
	all other information is already written*/
int	run_redir(t_redircmd *rcmd, t_tools *tool)
{
	rcmd->mode = check_file_type(rcmd, rcmd->fd);
	if (rcmd->mode == -1)
		exit(1); // exit fail
	// not sure about this - is a return enough in all cases
	else if (rcmd->mode == -1)
		return (0);
	close(rcmd->fd); // close(0)
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
	// opening at fd 0 if zero was closed
	if (rcmd->fd == -1)
	{
		print_errno_exit(NULL, strerror(errno), 0, tool);
	}
	handle_node(rcmd->cmd, tool);
	return (1); //(success)
}

int	run_pipeless_builtin_tree(t_cmd *cmd, t_tools *tool)
{
	t_execcmd	*ecmd;
	t_redircmd	*rcmd;

	if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		rcmd->mode = check_file_type(rcmd, rcmd->fd);
		if (rcmd->mode == -1)
			return (0);
		close(rcmd->fd);
		rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
		if (rcmd->fd == -1)
			return (0); // $?
		run_pipeless_builtin_tree(rcmd->cmd, tool);
	}
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		return (run_builtin(ecmd, tool));
	}
	return (0);
}

//**********************************************************************
// Broken Pipe Error (SIGPIPE): A broken pipe can
//  happen if the command on the reading end of the pipe terminates
// unexpectedly before the writing process finishes. In this case,
// the writing command (the one trying to send data to the pipe)
//  will receive a SIGPIPE signal and often terminate.
