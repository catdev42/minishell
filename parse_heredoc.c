/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/21 18:20:36 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// rewrite nullify to use actual information

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
			error_exit(tools, errno);
		ft_strlcpy(heredocs[i], "heredoc", MAXARGS);
		ft_strlcat(heredocs[i], tempalloc, MAXARGS);
		free(tempalloc);
		i++;
	}
	return ;
}

void	here_unlink(t_tools *tools)
{
	int	i;

	char(*heredocs)[20];
	heredocs = tools->heredocs;
	i = 0;
	while (i < MAXARGS)
	{
		if (access(heredocs[i], F_OK) == -1)
		{
			if (errno == ENOENT)
				errno = 0;
			else
				print_error(heredocs[i], "permission", NULL);
		}
		else
			unlink(heredocs[i]);
		i++;
	}
	return ;
}

// tools->lastredir;
// launchreadlineloop
// write what it sees into a file and turn it into a regular infile
int	createredir_here(char *delim, int mode, int fd, t_tools *tools)
{
	char	*end;
	char	*filename;
	int		len;

	end = get_token_end(delim);
	filename = make_heredoc_file(delim, tools);
	len = end - delim;
	if (tools->lastredir)
	{
		tools->lastredir->cmd = makeredir(filename, end, mode, fd);
		tools->lastredir = (struct s_redircmd *)tools->lastredir->cmd;
	}
	else
		tools->lastredir = (struct s_redircmd *)makeredir(filename, end, mode,
				fd);
	if (!tools->lastredir)
		error_exit(tools, 1);
	return (len);
}

/*Gives user the cursor - must check*/
char	*make_heredoc_file(char *delim, t_tools *tools)
{
	char *end;
	char *tempalloc_delim;
	char *line;
	char *templine;
	int fd;

	init_zero(NULL, NULL, &end, &tempalloc_delim);
	end = get_token_end(delim);
	fd = open(tools->heredocs[tools->hereindex++],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		error_exit(tools, errno);
	tempalloc_delim = ft_substr(delim, 0, end - delim);
	while (1)
	{
		// ft_putstr_fd(1, "\n"); // do we need this? test
		line = NULL;
		line = readline("heredoc: ");
		if (!line)
			error_exit(tools, errno);
		if (ft_strncmp(line, tempalloc_delim, end - delim + 1) == 0)
			break ;
		templine = safe_calloc(ft_strlen(line) + 3, 1, tools);
		ft_strlcpy(templine, line, ft_strlen(line) + 3);
		ft_strlcat(templine, "\n", ft_strlen(line) + 3);
		free(line);
		if (write(fd, templine, ft_strlen(templine)) == -1)
		{
			free(templine);
			error_exit(tools, errno);
		}
		free(templine);
	}
	free(line);
	return (tools->heredocs[tools->hereindex - 1]);
}