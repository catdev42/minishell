/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/28 07:04:33 by spitul           ###   ########.fr       */
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
	//char			*keep_red_start;

	ret = NULL;
	while (*start && start < end_of_exec)
	{
		append = 0;
		// mode = -1;
		fd_in_or_out = -1;
		if (isquote(*start))
			start += skip_quotes(start, 0);
		if (isredir(*start))
		{
			//keep_red_start = start;
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
					start++;
				createredir(&start[2], append, fd_in_or_out, tools);
			}
			if (!ret) // this only happens on the first finding of redir
				ret = (struct s_cmd *)tools->lastredir;
		}
		start++;
	}
	parseargv(tools->s, end_of_exec, tools);
	tools->lastredir = NULL;
	return ((struct s_cmd *)ret);
}

// struct s_cmd	*parse_redirs(char *start, char *end_of_exec, t_tools *tools)
// {
// 	int				fd_in_or_out;
// 	int				mode;
// 	struct s_cmd	*ret;

// 	ret = NULL;
// 	while (*start && start < end_of_exec)
// 	{
// 		mode = -1;
// 		fd_in_or_out = -1;
// 		if (isquote(*start))
// 			start += skip_quotes(start, 0);
// 		if (isredir(*start))
// 		{
// 			fd_in_or_out = infile_or_outfile(start);
// 			if (start[0] == start[1] && start[0] == '<')
// 			{
// 				createredir_here(&start[2], mode, 0, tools);
// 				start = start + skip_token(start, 0);
// 			}
// 			else
// 			{
// 				if (start[1] == start[0])
// 					start++;
// 				ret = createredir(++start, mode, fd_in_or_out, tools);
// 				// (struct t_execcmd *)ret->appe
// 			}
// 			if (!ret)
// 				ret = (struct s_cmd *)tools->lastredir;
// 		}
// 		start++;
// 	}
// 	parseargv(tools->s, end_of_exec, tools);
// 	tools->lastredir = NULL;
// 	return ((struct s_cmd *)ret);
// }

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
		error_exit(tools, UNKNOWNERROR);
	return (len);
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
		if (index == MAXARGS)
		{
			print_error("argv", "too many arguments", NULL);
			return (NULL);
		}
		while (start[i] && isspace(start[i]))
			i++;
		if (start[i] && istoken(start[i]))
			i = skip_token(start, i);
		// while (start[i] && !isspace)
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
