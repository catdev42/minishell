/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:28 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/10 16:18:37 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	*clean_line(char *line, int linelen, t_tools *tools)
{
	char	*c_line;
	size_t	i;
	size_t	j;

	init_zero(&i, &j, &c_line, NULL);
	tools->cl_capacity = linelen * 2;
	tools->cleanline = safe_calloc(tools->cl_capacity + 2, 1, tools);
	c_line = tools->cleanline;
	while (line[i] && j < tools->cl_capacity)
	{
		// CHECK WHAT ALL THE COPY FUNCTIONS RETURN cause if copy
		// var doesn't increment... we are in infinite loop
		if (line[i] == '\'' || line[i] == '"')
			i = i + copy_quotes(&c_line[j], &line[i], tools);
		else if (line[i] == '|')
			i = i + copy_pipe(&c_line[j], &line[i], i);
		else if (line[i] == '>' || line[i] == '<')
			i = i + copy_redirect(&c_line[j], &line[i], i);
		else if (ft_isspace(line[i]))
			i = i + copy_spaces(&c_line[j], &line[i]);
		else if (line[i] == '$' && line[i - 1] != '\\' && line[i + 1] != ' ')
			i = i + copy_var(&c_line[j], &line[i], tools);
		else
			c_line[j++] = line[i++];
		c_line = tools->cleanline;
		j = ft_strlen(c_line);
	}
	// remove_useless_quotes(tools->cleanline);
	return (c_line);
}

int	copy_spaces(char *c_line, char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (ft_isspace(line[i]))
	{
		c_line[j++] = line[i++];
		while (ft_isspace(line[i]))
			i++;
	}
	return (i);
}

int	copy_quotes(char *c_line, char *line, t_tools *tools)
{
	char	quote_char;
	int		i;
	int		j;

	i = 0;
	j = 0;
	quote_char = line[i];
	c_line[j++] = line[i++];
	while (line[i] && line[i] != quote_char)
	{
		if (line[i] == '$' && quote_char == '\"' && line[i - 1] != '\\'
			&& line[i + 1] != ' ')
		{
			i = i + copy_var(&c_line[j], &line[i], tools);
			c_line = tools->cleanline;
			while (c_line[j])
				j++;
		}
		else
			c_line[j++] = line[i++];
	}
	c_line[j++] = line[i++];
	i = i + copy_spaces(&c_line[j], &line[i]);
	return (i);
}
// if (line[i] == 0)
// 	print_error("unclosed quotes, please try again", NULL);

int	copy_pipe(char *c_line, char *line, int current_line_index)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if (current_line_index > 0)
		if (!ft_isspace(c_line[j - 1]))
			c_line[j++] = ' ';
	c_line[j++] = line[i++];
	if (!ft_isspace(line[i]))
		c_line[j++] = ' ';
	i = i + copy_spaces(&c_line[j], &line[i]);
	return (i);
}

// TODO add heredoc here
int	copy_redirect(char *c_line, char *line, int current_line_index)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if (current_line_index > 0)
		if (c_line[i - 1] != ' ')
			c_line[j++] = ' ';
	while (line[i] == '>' || line[i] == '<')
		c_line[j++] = line[i++];
	if (line[i - 1] == '>' || line[i - 1] == '<')
		while (ft_isspace(line[i]))
			i++;
	i = i + copy_spaces(&c_line[j], &line[i]);
	return (i);
}
