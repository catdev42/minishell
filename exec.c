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
		signal_init_sa(tools->sa, handle_printn_sig);
		pid = fork();
		if (pid == -1)
			print_errno_exit(NULL, NULL, 0, tools);
		if (pid == 0)
		{
			signal_init_sa(tools->sa, SIG_DFL);
			handle_node(tools->tree, tools);
		}
		waitpid(pid, &status, 0);
		check_system_fail(status, tools, 0);
	}
	else
		run_pipeless_builtin_tree(tools->tree, tools);
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
	print_errno_exit("fork", "unknown error", 141, tool);
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
		handle_node(pcmd->left, tools);
	}
	pid2 = fork();
	if (pid2 == -1)
		print_errno_exit(NULL, NULL, 0, tools);
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		handle_node(pcmd->right, tools);
	}
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(pid1, &status1, 0);
	check_system_fail(status1, tools, 0);
	waitpid(pid2, &status2, 0);
	check_system_fail(status2, tools, 0);
	exit_with_code(tools, tools->exit_code);
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
		print_errno_exit(NULL, strerror(errno), 0, tool);
	}
	handle_node(rcmd->cmd, tool);
	return (1);
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
		{
			record_exit(1, tool);
			return (0);
		}
		close(rcmd->fd);
		rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
		if (rcmd->fd == -1)
		{
			record_exit(1, tool);
			return (0);
		}
		run_pipeless_builtin_tree(rcmd->cmd, tool);
	}
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		return (run_builtin(ecmd, tool));
	}
	return (0);
}
