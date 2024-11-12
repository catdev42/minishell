/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lineutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 23:59:13 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 16:08:30 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	extend_cleanline(t_tools *tools, int add);

/* EXPANDS: Returns len of variable in line (including the $ or not?) */
/* This is an insanely confusing function - rewrite? */
int	copy_var(char *c_line, char *line, t_tools *tools)
{
	char	*var_alloc;
	char	*var_result;
	int		i;
	int		curr_cl_ind;

	var_alloc = NULL;
	curr_cl_ind = c_line - tools->cl;
	i = 1;
	while (line[i] && !ft_isspace(line[i]) && !isquote(line[i])
		&& !istoken(line[i]) && line[i] != '$')
		i++;
	if (i == 1)
	{
		c_line[0] = '$';
		return (i);
	}
	var_alloc = ft_substr(line, 1, i - 1);
	if (!var_alloc)
		error_exit_main(tools, 1);
	if (!ft_strncmp(var_alloc, "?", 1))
	{
		var_result = tools->exit_string;
		i = 2;
	}
	else
		var_result = get_var_value(tools->env, var_alloc);
	free(var_alloc);
	if (!var_result)
		return (i);
	if (tools->cl_capacity < ft_strlen(var_result)
		+ ft_strlen(tools->cl))
	{
		extend_cleanline(tools, ft_strlen(var_result));
		c_line = &(tools->cl[curr_cl_ind]);
	}
	ft_strlcpy(c_line, var_result, tools->cl_capacity - curr_cl_ind);
	return (i);
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
