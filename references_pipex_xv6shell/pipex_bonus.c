/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:45:09 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/19 19:04:55 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int argc, char **argv, char **env)
{
	char	cmdpath[1025];
	int		i;
	int		pid;
	int		fdfiles[2];

	if (argc < 5)
		return (pip_error(1));
	i = 2;
	fdfiles[0] = open(argv[1], O_RDONLY);
	if (fdfiles[0] < 0)
		error_handler_exit(argv[1]);
	fdfiles[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fdfiles[1] < 0)
		error_handler_exit(argv[argc - 1]);
	dup2(fdfiles[0], 0);
	close(fdfiles[0]);
	while (i < argc - 2)
		exec_pipe(argv[i++], env, cmdpath);
	dup2(fdfiles[1], 1);
	close(fdfiles[1]);
	pid = fork();
	if (pid == 0)
		execute(argv[argc - 2], env, cmdpath);
	return (0);
}

void	exec_pipe(char *cmd, char **env, char *cmdpath)
{
	int	pfd[2];
	int	pid;

	if (pipe(pfd) == -1)
		error_handler_exit(cmd);
	pid = fork();
	if (pid == -1)
		error_handler_exit(cmd);
	if (pid == 0)
	{
		close(pfd[0]);
		dup2(pfd[1], 1);
		close(pfd[1]);
		execute(cmd, env, cmdpath);
	}
	else
	{
		close(pfd[1]);
		dup2(pfd[0], 0);
		close(pfd[0]);
		waitpid(pid, NULL, 0);
	}
}

void	execute(char *cmd, char **env, char *cmdpath)
{
	char	*path;
	char	**command;

	path = ft_findpath(cmd, env, cmdpath);
	if (!path)
	{
		perror("pipex:");
		exit(1);
	}
	command = ft_split(cmd, ' ');
	if (execve(path, command, env) == -1)
	{
		ft_freetab(command);
		error_handler_exit(cmd);
	}
}

char	*ft_findpath(char *cmd, char **env, char *cmdpath)
{
	int		i;
	char	**splitpaths;
	char	*path;
	char	**splitcommand;

	path = ft_findpathenv("PATH", env);
	if (!path)
		return (cmd);
	splitpaths = ft_split(path, ':');
	splitcommand = ft_split(cmd, ' ');
	i = 0;
	while (splitpaths[i])
	{
		path = ft_jointhree(cmdpath, splitpaths[i++], "/", splitcommand[0]);
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_freetab(splitpaths);
			ft_freetab(splitcommand);
			return (path);
		}
		ft_bzero(cmdpath, 1024);
	}
	ft_freetab(splitpaths);
	ft_freetab(splitcommand);
	return (cmd);
}

char	*ft_findpathenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*tempsubstr;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		tempsubstr = ft_substr(env[i], 0, j);
		if (ft_strncmp(name, tempsubstr, 4) == 0)
		{
			free(tempsubstr);
			return (env[i] + j + 1);
		}
		free(tempsubstr);
		i++;
	}
	return (NULL);
}

/*
error_handler function
char	*firstw;

firstw = first_word(str, ' ');
if	(ft_strncmp(firstw, " ", 2) == 0)
free(firstw);


void	exec_pipe(int *p_fd, char *cmd, char **env, char *cmdpath)
{
pid_t	pid;

// int		p_fd[2];
if	(pipe(p_fd) == -1)
	exit(0);
pid = fork();
if (pid == -1)
	exit(0);
if (!pid)
{
	// child process assigns the write end to out
	close(p_fd[0]);
	dup2(p_fd[1], 1);
	execute(cmd, env, cmdpath);
}
else
{

	// parent process asssigns the read end to std_in and
	// doesn't evaporate next function will automatically
	// read from std in which is now the read end of the pipe

	close(p_fd[1]);
	dup2(p_fd[0], 0);
}
}

int		fdfiles[2];

int		fdfiles[2];

fdfiles[0] = open(argv[1], O_RDONLY);
fdfiles[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
if (fdfiles[0] < 0 || fdfiles[1] < 0)
return (pip_error(2));


pid1 = fork();
if (pid1 == -1)
	return (4);
if (pid1 == 0)
	child_process(argv, p_fd, env, fd_in, cmdpath);
dup2(fd_out, 1);
dup2(p_fd[0], 0);
close(p_fd[1]);
close(p_fd[0]);
execute(argv[3], env, cmdpath);




while (i < (basics->argc) - 3)
{
if (pfd[i][0] != fd_read)
	close(pfd[i][0]);
if (pfd[i][1] != fd_write)
	close(pfd[i][1]);
}
*/