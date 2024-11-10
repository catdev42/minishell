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
executes without forking*/

int	running_msh(t_tools *tool)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!tool->tree)
		return (0);
	if ((tool->tree->type == PIPE) || (tool->tree->type != PIPE
			&& (builtin_check_walk(tool->tree) == 0)))
	{
		pid = fork();
		if (pid == -1)
			print_errno_exit(NULL, NULL, 0, tool); // myakoven system fail
		if (pid == 0)
		{
			tool->sa->sa_handler = SIG_DFL; // NEW TODO
			handle_node(tool->tree, tool);
		}
		waitpid(pid, &status, 0);
		check_system_fail(status, tool, 1); // maykoven this also exits
	}
	else
		run_pipeless_builtin_tree(tool->tree, tool);
	return (1);
}

int	handle_node(t_cmd *cmd, t_tools *tool)
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
	printf("\nhier we have the exit code in handle node %d\n", tool->exit_code);
	return (tool->exit_code); //the parent process does return here? - 
		// for example after the run_pipe is done - execve will be exiting the children

}

/* function forks and sets up and manages pipes*/

void	run_pipe(t_pipecmd *pcmd, t_tools *tool)
{
	int		pipefd[2];
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		print_errno_exit(NULL, NULL, 0, tool);
	pid1 = fork();
	if (pid1 == -1)
		print_errno_exit(NULL, NULL, 0, tool);
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		handle_node(pcmd->left, tool); // terminating
	}
	// close(pipefd[1]);
	pid2 = fork();
	if (pid2 == -1)
		print_errno_exit(NULL, NULL, 0, tool);
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		handle_node(pcmd->right, tool); // terminating
	}
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(pid1, &status1, 0);
	check_system_fail(status1, tool, 0);
	waitpid(pid2, &status2, 0);
	check_system_fail(status2, tool, 0);
	// ft_putstr_fd("test", 2);
	good_exit(tool);
}

/* checks if the file exists and 
sets the redirection by opening the file at the required fd */
int	run_redir(t_redircmd *rcmd, t_tools *tool)
{
	rcmd->mode = check_file_type(rcmd, rcmd->fd);
	// MYAKOVEN: IF NOT A VALID REDIR: EXIT FORK
	// error is already printed
	if (rcmd->mode == -1)
	{
		error_exit_main(tool, 1);
	}
	close(rcmd->fd);
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
	if (rcmd->fd == -1)
	{
		print_errno_exit(NULL, strerror(errno), 1, tool);
	}
	handle_node(rcmd->cmd, tool);
	return (0); //(success)
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
			return (1);
		close(rcmd->fd);
		rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
		if (rcmd->fd == -1)
			return (1); // $?
		run_pipeless_builtin_tree(rcmd->cmd, tool);
	}
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		return (run_builtin(ecmd, tool));
	}
	return (0);
}
