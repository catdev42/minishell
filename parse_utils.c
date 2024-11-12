/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 04:30:08 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"


int	infile_or_outfile(char *start)
{
	if (*start == '<')
		return (0);
	else if (*start == '>')
		return (1);
	return (-1);
}

/* Gets pointer to the space after the token */
char	*get_token_end(char *namestart)
{
	int	i;

	i = 0;
	while (namestart[i] && !ft_isspace(namestart[i]))
	{
		if (isquote(namestart[i]))
			i = skip_quotes(namestart, i);
		i++;
	}
	return (&namestart[i]);
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

void	nullify(char *cline, t_tools *tools)
{
	int	i;

	i = 0;
	while (&cline[i] < tools->e_cline)
	{
		if (ft_isspace(cline[i]))
			cline[i] = 0;
		else
			i = skip_token(cline, i);
		i++;
	}
}

/* Provide the line and its capacity */
void	remove_useless_quotes_final(char *cline, size_t linecapacity)
{
	size_t	i;
	char	quotechar;
	char	*firstquote;

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
				i++;
			}
			if (cline[i] && cline[i] == quotechar)
				i -= remove_two(firstquote, &cline[i]);
		}
		i++;
	}
}
