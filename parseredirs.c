/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseredirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/18 22:26:40 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

struct s_cmd	*parseexec(char *start, char *end_of_exec, t_tools *tools)
{
	struct s_cmd	*ret;

	ret = NULL;
	if (peek(start, end_of_exec, REDIR))
	{
		ret = parse_redirs(start, end_of_exec, tools);
		if (!ret)
			return (NULL);
	}
	if (!ret)
		ret = parseargv(start, end_of_exec, tools);
	return (ret);
}

struct s_cmd	*parse_redirs(char *start, char *end_of_exec, t_tools *tools)
{
	int				fd_in_or_out;
	int				mode;
	struct s_cmd	*ret;

	ret = NULL;
	while (*start && start < end_of_exec)
	{
		mode = 0;
		fd_in_or_out = -1;
		if (isquote(*start))
			start += skip_quotes(start, 0);
		if (isredir(*start))
		{
			fd_in_or_out = infile_or_outfile(start);
			
			if (start[1] == start[0])
				start++;
			createredir(++start, mode, fd_in_or_out, tools);
			if (!ret)
				ret = (struct s_cmd *)tools->lastredir;
		}
		start++;
	}
	parseargv(tools->s, end_of_exec, tools);
	tools->lastredir = NULL;
	return ((struct s_cmd *)ret);
}

struct s_cmd	*createredir(char *filestart, int mode, int fd, t_tools *tools)
{
	char	*end;

	end = NULL;
	end = get_token_end(filestart);
	if (tools->lastredir)
	{
		tools->lastredir->cmd = makeredir(filestart, end, mode, fd);
		tools->lastredir = (struct s_redircmd *)tools->lastredir->cmd;
	}
	else
		tools->lastredir = (struct s_redircmd *)makeredir(filestart, end, mode,
				fd);
	if (!tools->lastredir)
		error_exit(tools, 1);
	return ((struct s_cmd *)tools->lastredir);
}

struct s_cmd	*parseargv(char *start, char *end, t_tools *tools)
{
	struct s_execcmd	*ecmd;
	int					i;
	int					index;

	ecmd = NULL;
	ecmd = (struct s_execcmd *)makeexec();
	i = 0;
	index = 0;
	if (!ecmd)
		error_exit(tools, 1);
	while (start[i] && (&start[i] < end))
	{
		while (start[i] && isspace(start[i]))
			i++;
		if (start[i] && istoken(start[i]))
			i = skip_token(start, i);
		else if (start[i])
		{
			ecmd->argv[index] = &start[i];
			i = skip_token(start, i);
			ecmd->eargv[index++] = &start[i + 1];
		}
		i++;
	}
	if (tools->lastredir)
		tools->lastredir->cmd = (struct s_cmd *)ecmd;
	return ((struct s_cmd *)ecmd);
}

/* parseexex() > (peek redir) > parse_redirs > parse_exec > connect things */
