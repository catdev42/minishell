/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:01:36 by spitul            #+#    #+#             */
/*   Updated: 2024/11/12 20:28:33 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*
Checks the exit code, determine if its a fatal errno, and exits.
In main status is caught and we need to determine if to close the whole program
*/
void	check_system_fail(int status, t_tools *tool, bool inmain)
{
	int	sig;

	if (WIFEXITED(status))
	{
		sig = WEXITSTATUS(status);
		record_exit(sig, tool);
		if (inmain && sig == 142)
			error_exit_main(tool, 1);
		else if (sig == SYSTEMFAIL || sig == ENOMEM || sig == EPIPE
			|| sig == EMFILE || sig == EBADF || sig == EFAULT || sig == ENOSPC
			|| sig == EIO || sig == ENODEV)
			error_exit_main(tool, tool->exit_code);
		else if (sig != 0 && sig != 2 && sig != 126 && sig != 127)
			record_exit(1, tool);
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		record_exit(sig + 128, tool);
		if (sig == SIGSEGV || sig == SIGBUS || sig == SIGFPE || sig == SIGILL
			|| sig == SIGABRT || sig == SIGSYS)
			error_exit_main(tool, record_exit(sig + 128, tool));
	}
	return ;
}

/*
Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0
If we deal with an outfile, there is no problem if file is not found
*/
int	file_dir_noexist(const char *path, int fd_in_or_out)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		if (fd_in_or_out == 1 && errno == ENOENT)
			return (1);
		else
		{
			print_error(path, NULL, strerror(errno), NULL);
			return (0);
		}
	}
	if (S_ISREG(path_stat.st_mode))
	{
		return (1);
	}
	else if (S_ISDIR(path_stat.st_mode))
		return (2);
	else
		print_error(path, NULL,"Is neither a file nor a directory", NULL);
	return (0);
}

char	*check_cmd_path(char *path, t_execcmd *cmd, t_tools *tools)
{
	char	*cmdpath;
	char	*temp;

	cmdpath = NULL;
	temp = NULL;
	temp = ft_strjoin(path, "/");
	if (!temp)
		print_errno_exit(NULL, NULL, 0, tools);
	cmdpath = ft_strjoin(temp, cmd->argv[0]);
	free(temp);
	if (!cmdpath)
		print_errno_exit(NULL, NULL, 0, tools);
	if (access(cmdpath, F_OK) == 0)
	{
		if (access(cmdpath, X_OK) != 0)
		{
			free(cmdpath);
			print_errno_exit(NULL, NULL, 0, tools);
		}
		return (cmdpath);
	}
	free(cmdpath);
	return (NULL);
}

/* Return the MODE necessary for OPEN() file or dir */
int	check_file_type(t_redircmd *rcmd, int fd_in_or_out)
{
	int	fileordir;

	if (!rcmd || fd_in_or_out < 0)
		return (0);
	fileordir = file_dir_noexist(rcmd->file, fd_in_or_out);
	if (fileordir == 0)
		return (-1);
	if (fileordir == 2 && rcmd->fd == 1)
	{
		print_error(rcmd->file,NULL, "is a directory", NULL);
		return (-1);
	}
	if (fileordir == 1 && rcmd->append && rcmd->fd == 1)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (fileordir == 1 && !rcmd->append && rcmd->fd == 1)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (fileordir == 1 && rcmd->fd == 0)
		return (O_RDONLY);
	else if (fileordir == 2 && rcmd->fd == 0)
		return (O_RDONLY | O_DIRECTORY);
	return (0);
}
