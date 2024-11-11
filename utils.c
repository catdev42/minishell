/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:15:14 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 20:19:48 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// check the entire line for quotes with 0 returns if its not valid and 1 if it is
int	val_quts(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (isquote(line[i]))
		{
			if (!check_quotes(line, i))
				return (0);
			i = skip_quotes(line, i);
		}
		i++;
	}
	return (1);
}

/* 	CHECKS IF THE COMMAND LINE IS EMPTY
	Returns 1 if there is anything except spaces in a line */
int	val_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (1);
		i++;
	}
	return (0);
}

int	istoken(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
int	isredir(char c)
{
	return (c == '<' || c == '>');
}

int	isquote(char c)
{
	return (c == '\'' || c == '\"');
}
