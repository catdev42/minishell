/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 00:42:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 22:43:29 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

struct s_cmd	*parseline(char *cline, t_tools *tools)
{
	struct s_cmd	*left;
	struct s_cmd	*right;

	tools->s = cline;
	while (peek(tools->s, tools->e_cline, PIPE))
	{
		left = NULL;
		right = NULL;
		tools->cmd_end = peek(tools->s, tools->e_cline, PIPE);
		if (!create_pipe_node(left, right, tools))
			return (0);
	}
	if (!tools->tree)
		tools->tree = parseexec(tools->s, tools->e_cline, tools);
	if (!tools->tree)
		return (0);
	nullify(tools->cl, tools);
	remove_useless_quotes_final(tools->cl, tools->cl_capacity);
	return (tools->tree);
}

int	create_pipe_node(struct s_cmd *left, struct s_cmd *right, t_tools *tools)
{
	left = parseexec(tools->s, tools->cmd_end, tools);
	if (!left)
	{
		return (0);
	}
	tools->s = tools->cmd_end + 1;
	if (peek(tools->s, tools->e_cline, PIPE))
		right = NULL;
	else
	{
		right = parseexec(tools->s, tools->e_cline, tools);
		if (!right)
		{
			clean_two(left, NULL);
			return (0);
		}
	}
	if (!createpipe(left, right, tools))
		return (0);
	return (1);
}

/* Helper for parseline and pipe creation:
determines where to attach the pipe */
struct s_cmd	*createpipe(struct s_cmd *left, struct s_cmd *right,
		t_tools *tools)
{
	if (tools->lastpipe)
	{
		tools->lastpipe->right = makepipe(left, right);
		tools->lastpipe = (struct s_pipecmd *)tools->lastpipe->right;
	}
	else
		tools->lastpipe = (struct s_pipecmd *)makepipe(left, right);
	if (!tools->lastpipe)
	{
		clean_two(left, right);
		error_exit_main(tools, 1);
	}
	if (!tools->tree)
		tools->tree = (struct s_cmd *)tools->lastpipe;
	return ((struct s_cmd *)tools->lastpipe);
}

char	*peek(char *line, char *end, int token)
{
	char	*tokenaddress;
	int		i;

	tokenaddress = 0;
	i = 0;
	while (line[i] && &line[i] < end)
	{
		if (isquote(line[i]))
			i = skip_token(line, i);
		if (istoken(line[i]))
		{
			if ((isredir(line[i]) && token == REDIR) || (line[i] == '|'
					&& token == PIPE))
			{
				tokenaddress = &line[i];
				break ;
			}
		}
		else if (token == ALPHA && ft_isspace(line[i]) && !istoken(line[i + 1]))
			return (&line[i + 1]);
		++i;
	}
	return (tokenaddress);
}
