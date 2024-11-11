/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 22:43:12 by myakoven         ###   ########.fr       */
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

char	*clean_line_expand_only(char *line, int linelen, t_tools *tools)
{
	char	*c_line;
	size_t	i;
	size_t	j;

	init_zero(&i, &j, &c_line, NULL);
	tools->cl_capacity = linelen * 2;
	tools->cl = safe_calloc(tools->cl_capacity + 2, 1, tools);
	c_line = tools->cl;
	while (line[i] && j < tools->cl_capacity)
	{
		if (line[i] == '\'' || line[i] == '"')
			i = i + copy_quotes(&c_line[j], &line[i], tools);
		else if (line[i] == '$' && line[i - 1] != '\\' && line[i + 1] != ' ')
			i = i + copy_var(&c_line[j], &line[i], tools, 1);
		else
			c_line[j++] = line[i++];
		j = ft_strlen(c_line);
		c_line = tools->cl;
	}
	return (c_line);
}
