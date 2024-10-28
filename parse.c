/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 00:42:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/28 14:01:54 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void		nullify(char *cline, t_tools *tools);
static void		remove_useless_quotes_final(char *cline, size_t linecapacity);
// FIX THIS TO BE MORE PRECISE MAYBE?

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
		left = parseexec(tools->s, tools->cmd_end, tools);
		if (!left)
			return (NULL);
		// ERROR MANAGEMENT:
		// (print error message in parseexec(),
		// give cursor to user in loop)
		tools->s = tools->cmd_end + 1;
		if (peek(tools->s, tools->e_cline, PIPE))
			right = NULL;
		else
		{
			right = parseexec(tools->s, tools->e_cline, tools);
			if (!right)
				return (clean_two(left, NULL));
			// ERROR MANAGEMENT:
			// (print error message in parseexec(),
			// give cursor to user in loop)
		}
		createpipe(left, right, tools);
		// pipe creation (exits from there if malloc error)
	}
	if (!tools->tree)
		tools->tree = parseexec(tools->s, tools->e_cline, tools);
	nullify(tools->cleanline, tools);
	remove_useless_quotes_final(tools->cleanline, tools->cl_capacity);
	return (tools->tree);
}

/* Helper for parseline and pipe creation: determines where to attach the pipe */
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
		error_exit_main(tools, 1); // EXITS ENTIRE PROGRAM ON ALLOCATION ERROR!
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
			i = skip_quotes(line, i);
		if (istoken(line[i]))
		{
			if (isredir(line[i]) && token == REDIR)
			{
				tokenaddress = &line[i];
				break ;
			}
			if (line[i] == '|' && token == PIPE)
			{
				tokenaddress = &line[i];
				break ;
			}
		}
		i++; // TODO test SIGNAL HANDLER command
	}
	return (tokenaddress);
}

static void	nullify(char *cline, t_tools *tools)
{
	int	i;

	i = 0;
	while (&cline[i] < tools->e_cline)
	{
		if (isspace(cline[i]))
			cline[i] = 0;
		else
			i = skip_token(cline, i);
		i++;
	}
}

// static void	remove_useless_quotes_nulled(char *cline, t_tools *tools)
// {
// 	size_t	i;
// 	char	quotechar;
// 	char	*firstquote;
// 	char	*secondquote;
// 	bool	removequotes;

// 	removequotes = 1;
// 	firstquote = 0;
// 	secondquote = 0;
// 	i = 0;
// 	while (i < tools->cl_capacity)
// 	{
// 		quotechar = 0;
// 		removequotes = 1;
// 		if (isquote(cline[i]))
// 		{
// 			quotechar = cline[i];
// 			firstquote = &cline[i];
// 			i++;
// 			while (cline[i] && cline[i] != quotechar)
// 				i++;
// 			if (cline[i] == quotechar)
// 			{
// 				secondquote = &cline[i];
// 				remove_two(firstquote, secondquote);
// 				i -= 2;
// 			}
// 		}
// 		i++;
// 	}
// }

/* Provide the line and its capacity */
static void	remove_useless_quotes_final(char *cline, size_t linecapacity)
{
	size_t	i;
	char	quotechar;
	char	*firstquote;

	// bool	removequotes;
	// removequotes = 0;
	i = 0;
	while (i < linecapacity)
	{
		firstquote = NULL;
		quotechar = 0;
		if (isquote(cline[i]))
		{
			quotechar = cline[i];
			firstquote = &cline[i];
			i++;
			while (cline[i] && cline[i] != quotechar)
			{
				// if (ft_isspace(cline[i]) || istoken(cline[i]))
				// 	removequotes = 1;
				i++;
			}
			if (cline[i] && cline[i] == quotechar) // && removequotes
				i -= remove_two(firstquote, &cline[i]);
		}
		i++;
	}
}
