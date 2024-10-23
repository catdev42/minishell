/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lineutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 23:59:13 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/23 18:35:52 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	extend_cleanline(t_tools *tools, int add);

/* EXPANDS: Returns len of variable in line */
/* This is an insanely confusing function - rewrite? */
int	copy_var(char *c_line, char *line, t_tools *tools)
{
	char	*var;
	char	*var_result;
	int		i;
	int		curr_cl_ind;

	var = NULL;
	curr_cl_ind = c_line - tools->cleanline;
	i = 1;
	while (line[i] && !ft_isspace(line[i]) && !isquote(line[i])
		&& !istoken(line[i]) && line[i] != '$')
		i++;
	if (i == 1)
		return (i);
	var = ft_substr(line, 1, i - 1);
	if (!var)
		error_exit(tools, 1);
	var_result = get_var_value(tools->env, var);
	free(var);
	if (!var_result)
		return (i);
	if (tools->cl_capacity < ft_strlen(var_result)
		+ ft_strlen(tools->cleanline))
	{
		extend_cleanline(tools, ft_strlen(var_result));
		c_line = &(tools->cleanline[curr_cl_ind]);
	}
	ft_strlcpy(c_line, var_result, tools->cl_capacity - curr_cl_ind);
	return (i);
}

static void	extend_cleanline(t_tools *tools, int add)
{
	int		new_cl_len;
	char	*tmpold;

	new_cl_len = tools->cl_capacity + add;
	tmpold = tools->cleanline;
	tools->cleanline = ft_calloc(new_cl_len + 2, 1);
	if (!tools->cleanline)
		error_exit(tools, 1);
	tools->cl_capacity = new_cl_len;
	ft_strlcpy(tools->cleanline, tmpold, tools->cl_capacity);
	free(tmpold);
}

void	remove_useless_quotes(char *cline)
{
	size_t	i;
	char	quotechar;
	char	*firstquote;
	bool	removequotes;

	i = 0;
	while (cline[i])
	{
		firstquote = NULL;
		quotechar = 0;
		removequotes = 1;
		if (isquote(cline[i]))
		{
			quotechar = cline[i];
			firstquote = &cline[i];
			while (cline[++i] && cline[i] != quotechar)
				if (ft_isspace(cline[i]) || istoken(cline[i]))
					removequotes = 0;
			if (removequotes && cline[i] == quotechar)
				i -= remove_two(firstquote, &cline[i]);
		}
		i++;
	}
}

/*
Removes 2 characters from a string, rewriting the string in the process.
Used to remove quotes in this program.
Returns the number of characters removed.
*/
int	remove_two(char *first, char *second)
{
	int	i;

	i = 0;
	// printf("im in remove 2\n");
	if (second)
	{
		// printf(" %p - nothing???", second);
		ft_memmove(second, second + 1, ft_strlen(second + 1) + 1);
		i++;
	}
	if (first)
	{
		ft_memmove(first, first + 1, ft_strlen(first + 1) + 1);
		i++;
	}
	return (i);
}
