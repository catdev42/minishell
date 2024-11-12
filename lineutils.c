/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lineutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 23:59:13 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 21:56:56 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	extend_cleanline(t_tools *tools, int add);
static int	check_cleanline_capacity(t_tools *tools, char *var_result);
static int	write_dollar(char *c_line);

/* EXPANDS: Returns len of variable in line (including the $ or not?) */
/* This is an insanely confusing function - rewrite? */

int	copy_var(char *c_line, char *line, t_tools *tools, int i)
{
	char	*var_alloc;
	char	*var_result;
	int		curr_cl_ind;

	var_alloc = NULL;
	curr_cl_ind = c_line - tools->cl;
	while (line[i] && !ft_isspace(line[i]) && !ft_strchr("'\"<|>$", line[i]))
		i++;
	if (i == 1)
		return (write_dollar(c_line));
	var_alloc = ft_substr(line, 1, i - 1);
	if (!var_alloc)
		error_exit_main(tools, 1);
	if (!ft_strncmp(var_alloc, "?", 1))
		var_result = tools->exit_string;
	else
		var_result = get_var_value(tools->env, var_alloc);
	if (var_result == tools->exit_string)
		i = 2;
	free(var_alloc);
	if (var_result && !check_cleanline_capacity(tools, var_result))
		c_line = &(tools->cl[curr_cl_ind]);
	if (var_result)
		ft_strlcpy(c_line, var_result, tools->cl_capacity - curr_cl_ind);
	return (i);
}

static int	write_dollar(char *c_line)
{
	c_line[0] = '$';
	return (1);
}

static int	check_cleanline_capacity(t_tools *tools, char *var_result)
{
	if (tools->cl_capacity < ft_strlen(var_result) + ft_strlen(tools->cl))
	{
		extend_cleanline(tools, ft_strlen(var_result));
		return (0);
	}
	return (1);
}

static void	extend_cleanline(t_tools *tools, int add)
{
	int		new_cl_len;
	char	*tmpold;

	new_cl_len = tools->cl_capacity + add;
	tmpold = tools->cl;
	tools->cl = ft_calloc(new_cl_len + 2, 1);
	if (!tools->cl)
		error_exit_main(tools, 1);
	tools->cl_capacity = new_cl_len;
	ft_strlcpy(tools->cl, tmpold, tools->cl_capacity);
	free(tmpold);
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
	if (second)
	{
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
