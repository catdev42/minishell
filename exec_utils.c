/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:01:36 by spitul            #+#    #+#             */
/*   Updated: 2024/11/07 06:47:22 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/**
 * this needs to checkthe exit code, determine if its a fatal errno, and exit...
 * exit codes in forks carry errno back!

	* in main status errno is caught and we need to determine to close the whole program or not
 */
void	check_system_fail(int status, t_tools *tool, bool inmain)
{
	int	sig;

	if (WIFEXITED(status))
	{
		sig = WEXITSTATUS(status);
		if (sig == 0)
		{
			ft_putstr_fd("hier 0\n", 2);
			record_exit(sig, tool);
			return ;
		}
		else if (inmain && sig == 142) // isnt being attributed nowhere in the programm
		{
			ft_putstr_fd("hier 1\n", 2);
			record_exit(1, tool);
			error_exit_main(tool, 1);
		}
		else if (sig == SYSTEMFAIL || sig == ENOMEM || sig == EPIPE
			|| sig == EMFILE || sig == EBADF || sig == EFAULT || sig == ENOSPC
			|| sig == EIO || sig == ENODEV)
			{ft_putstr_fd("hier 2\n", 2);error_exit_main(tool, tool->exit_code);}
		else
		{
			record_exit(1, tool);
			ft_putstr_fd("hier 3\n", 2);
			error_exit_main(tool, -4);
		}
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		record_exit(sig + 128, tool);
		if (sig == SIGKILL)
			return ;
		else if (sig == SIGSEGV || sig == SIGBUS || sig == SIGFPE
			|| sig == SIGILL || sig == SIGABRT || sig == SIGSYS)
		{
			// tool->exit_code = sig + 128;
			record_exit(sig + 128, tool);
			error_exit_main(tool, sig + 128);
		}
	}
	else
		return ;
	/*we dont exit unless the above, this is just an exra catcher for compiler
		*/// exit(0); // temporary?
}

/*

THIS IS MY  CALL OF check_file_type and FAILURE checker...
	mode = check_file_type(start, fd_in_or_out, tool); // TODO !!!
	if (mode == -1) //because 0 is returned for O_RDONLY
		return (NULL); return out or exit fork
You can do this in your code cause it should work for builtin in the main process...
*/
/* Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0*/
int	file_dir_noexist(const char *path, int fd_in_or_out)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		// if it is an outfile and path is not found, we return 1
		// because a regular file will be created
		if (fd_in_or_out == 1 && errno == ENOENT)
			return (1); // not an error
		else
		{ // an error
			print_error(path, strerror(errno), NULL);
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
		print_error(path, "Is neither a file nor a directory", NULL);
	return (0); // error
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
	if (fileordir == 2 && rcmd->fd == 1) // directory, outfile
	{
		print_error(rcmd->file, "is a directory", NULL);
		return (-1); //myakoven oct 28, we cant redirect this: TODO
	}
	if (fileordir == 1 && rcmd->append && rcmd->fd == 1)
		// reg file (not a directory), append, outfile
		// I HAVE TO ADDRESS APPEND IN REDIR CREATION (myakoven)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (fileordir == 1 && !rcmd->append && rcmd->fd == 1)
		// reg file (not a directory), trund, outfile
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (fileordir == 1 && rcmd->fd == 0)
		// reverted this to 1... cause regular file
		return (O_RDONLY);
	else if (fileordir == 2 && rcmd->fd == 0)
		// special condition for infile which is a directory
		return (O_RDONLY | __O_DIRECTORY);
	return (0);
}

/* from slack sde-silv (Shenya)
while (i < env->procs)
{
waitpid(env->arr[i].pid, &status, 0);
if	(WIFEXITED(status))
	env->arr[i].status = WEXITSTATUS(status);
else if	(WIFSIGNALED(status))
{
	sig = WTERMSIG(status);
	env->arr[i].status = 128 + sig;
}
i ++;
}
handle_sig_numbers(sig, status, env, i);
g_exit_status = env->arr[i - 1].status;
*/
