/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/12 04:18:12 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* ^C leave heredoc function and return to minishell
Create the files and record the name while in main process
Launch ONLY the heredoc readline in a separate process */
int	createredir_here(char *delim, int append, int fd, t_tools *tools)
{
	char	*end;
	char	*filename;
	int		len;

	end = get_token_end(delim);
	signal_init_sa(tools->sa, SIG_IGN);
	filename = make_heredoc_fork(delim, tools);
	if (!filename)
		return (0);
	len = end - delim;
	if (tools->lastredir)
	{
		tools->lastredir->cmd = makeredir(filename, end, append, fd);
		tools->lastredir = (struct s_redircmd *)tools->lastredir->cmd;
	}
	else
		tools->lastredir = (struct s_redircmd *)makeredir(filename, end, append,
				fd);
	if (!tools->lastredir)
		error_exit_main(tools, 1);
	signal_init_sa(tools->sa, handle_printn_sig);
	return (len);
}

/*Gives user the cursor - must check*/
char	*make_heredoc_fork(char *delim, t_tools *tools)
{
	char	*end;
	char	*tempalloc_delim;
	int		fd;
	pid_t	pid;

	pid = -1;
	init_zero(NULL, NULL, &end, &tempalloc_delim);
	end = get_token_end(delim);
	fd = open(tools->heredocs[tools->hereindex++],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		error_exit_main(tools, 1);
	tempalloc_delim = ft_substr(delim, 0, end - delim);
	if (!tempalloc_delim)
		error_exit_main(tools, 1);
	return (make_actual_fork(pid, tempalloc_delim, tools, fd));
}

char	*make_actual_fork(pid_t pid, char *allo_delim, t_tools *tools, int fd)
{
	int	status;

	status = 0;
	pid = fork();
	if (pid == -1)
		error_exit_main(tools, 1);
	if (pid == 0)
		write_heredoc(fd, allo_delim, tools);
	waitpid(pid, &status, 0);
	check_system_fail(status, tools, 1);
	if (g_signal == SIGINT || tools->exit_code == 2 || tools->exit_code == 130)
	{
		ft_putstr_fd("\n", 1);
		here_unlink(tools);
		free(allo_delim);
		close(fd);
		return (NULL);
	}
	record_exit(tools->exit_code, tools);
	close(fd);
	free(allo_delim);
	return (tools->heredocs[tools->hereindex - 1]);
}

void	write_heredoc(int fd, char *alloc_delim, t_tools *tools)
{
	signal_init_sa(tools->sa, SIG_DFL);
	free_things(&tools->cl, &tools->ln, NULL, -1);
	while (1)
	{
		tools->ln = readline("heredoc: ");
		if (!tools->ln || ft_strncmp(tools->ln, alloc_delim,
				ft_strlen(alloc_delim)) == 0)
		{
			if (!tools->ln)
				print_error("warning", "here-doc delimited by EOF, wanted ",
					alloc_delim);
			break ;
		}
		// tools->cl = clean_line_expand_only(tools->ln, ft_strlen(tools->ln),
		// 		tools);
		if (write(fd, tools->ln, ft_strlen(tools->ln)) == -1 || write(fd, "\n",
				1) == -1) //! tools->cl ||
		{
			free_things(NULL, NULL, &alloc_delim, fd);
			print_errno_exit(NULL, NULL, errno, tools);
		}
		free_things(&tools->ln, &tools->cl, NULL, -1);
	}
	free_things(NULL, NULL, &alloc_delim, fd);
	good_exit(tools);
}

/* Initialize the heredoc names struct */
void	here_init(char heredocs[MAXARGS][MAXARGS], t_tools *tools)
{
	int		i;
	char	*tempalloc;

	i = 0;
	while (i < MAXARGS)
	{
		tempalloc = NULL;
		tempalloc = ft_itoa(i);
		if (!tempalloc)
			error_exit_main(tools, errno);
		ft_strlcpy(heredocs[i], "heredoc", MAXARGS);
		ft_strlcat(heredocs[i], tempalloc, MAXARGS);
		free(tempalloc);
		i++;
	}
	return ;
}
