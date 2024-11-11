/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linesyntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 21:45:41 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 15:19:04 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static char	*get_redir_error(char *line, int i);
static int	check_first_pipe(char *line);
static int	check_pipe_sytax(char *line, int i);
static int	check_redir_syntax(char *line, int i);

int	val_red(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!check_first_pipe(line))
			return (print_error(NULL, UNEXP, "|"));
		if (line[i] == '\"' || line[i] == '\'')
			i = skip_quotes(line, i) + 1;
		if (line[i] && line[i] == '|')
		{
			if (!check_pipe_sytax(line, i))
				return (0);
		}
		else if (line[i] && istoken(line[i]))
		{
			if (line[i + 1] && line[i + 1] == line[i])
				i++;
			if (!check_redir_syntax(line, i))
				return (0);
		}
		if (line[i])
			i++;
	}
	return (1);
}

static int	check_pipe_sytax(char *line, int i)
{
	bool	hasalpha;

	hasalpha = 0;
	i++;
	while (line[i] && !istoken(line[i]))
	{
		if (!ft_isspace(line[i]) && !istoken(line[i]))
			hasalpha = 1;
		i++;
	}
	if (!hasalpha && line[i] == '|')
		return (print_error(NULL, UNEXP, "|"));
	else if (!hasalpha && !line[i])
		return (print_error(NULL, UNEXP, "newline"));
	return (1);
}

static int	check_redir_syntax(char *line, int i)
{
	bool	hasalpha;

	hasalpha = 0;
	i++;
	while (line[i] && !istoken(line[i]))
	{
		if (!ft_isspace(line[i]) && !istoken(line[i]))
			hasalpha = 1;
		i++;
	}
	if (!hasalpha && istoken(line[i]))
		return (print_error(NULL, UNEXP, (get_redir_error(line, i))));
	else if (!line[i] && !hasalpha)
		return (print_error(NULL, UNEXP, "newline"));
	return (1);
}

static char	*get_redir_error(char *line, int i)
{
	int	j;

	j = 0;
	while (istoken(line[i + j]) && j < 2)
		j++;
	if (istoken(line[i] == '|'))
		j = 1;
	if (istoken(line[i]) != istoken(line[i + 1]))
		j = 1;
	line[i + j] = 0;
	return (&line[i]);
}

// returns zero if syntax error with pipes in beginning or end
static int	check_first_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '|')
		return (0);
	return (1);
}

/*
	"<<|",
	"<<<<",
	">>>",
	"||",
	";;",
	"><",
	"<>",
*/
