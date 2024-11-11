/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 20:18:07 by myakoven         ###   ########.fr       */
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

/* we enter this only if we have redirs
we HAVE TO return a pointer to the branches...*/
struct s_cmd	*parse_redirs(char *start, char *end_of_exec, t_tools *tools)
{
	int				fd_in_or_out;
	bool			append;
	struct s_cmd	*ret;

	ret = NULL;
	while (*start && start < end_of_exec)
	{
		append = 0;
		fd_in_or_out = -1;
		if (isquote(*start))
			start += skip_quotes(start, 0);
		if (isredir(*start))
		{
			if (!actually_parse_redir(fd_in_or_out, start, tools, append))
				return (NULL);
			if (!ret)
				ret = (struct s_cmd *)tools->lastredir;
		}
		start++;
	}
	if (!parseargv(tools->s, end_of_exec, tools))
		return (NULL);
	tools->lastredir = NULL;
	return ((struct s_cmd *)ret);
}

int	actually_parse_redir(int fd_in_or_out, char *start, t_tools *tools,
		bool append)
{
	fd_in_or_out = infile_or_outfile(start);
	if (start[0] == start[1] && start[0] == '<')
	{
		if (createredir_here(&start[2], append, fd_in_or_out, tools) == 0)
			return (0);
		start += (get_token_end(start) - start);
	}
	else
	{
		if (start[1] == start[0])
		{
			start++;
			append = true;
		}
		createredir(++start, append, fd_in_or_out, tools);
		start += (get_token_end(start) - start);
	}
	return (1);
}

/*
RETURNS length of the token in line,
STORES the redir in either last redir or the last creted redir struct*/
int	createredir(char *filestart, int mode, int fd, t_tools *tools)
{
	char	*end;
	int		len;

	end = NULL;
	end = get_token_end(filestart);
	len = end - filestart;
	if (tools->lastredir)
	{
		tools->lastredir->cmd = makeredir(filestart, end, mode, fd);
		tools->lastredir = (struct s_redircmd *)tools->lastredir->cmd;
	}
	else
		tools->lastredir = (struct s_redircmd *)makeredir(filestart, end, mode,
				fd);
	if (!tools->lastredir)
		error_exit_main(tools, UNKNOWNERROR);
	return (len);
}

struct s_cmd	*parseargv(char *start, char *end, t_tools *tools)
{
	struct s_execcmd	*ecmd;
	int					i;
	int					index;

	ecmd = NULL;
	i = -1;
	index = 0;
	ecmd = (struct s_execcmd *)makeexec();
	if (!ecmd)
		error_exit_main(tools, 1);
	while (start && start[++i] && (&start[i] < end))
	{
		if (start[i] && istoken(start[i]))
			i = skip_token(start, i);
		else if (!isspace(start[i]))
		{
			ecmd->argv[index++] = &start[i];
			i = skip_token(start, i);
		}
		if (index == MAXARGS - 1)
			print_error("argv", "too many arguments", NULL);
	}
	if (tools->lastredir)
		tools->lastredir->cmd = (struct s_cmd *)ecmd;
	return ((struct s_cmd *)ecmd);
}
