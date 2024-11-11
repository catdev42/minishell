/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:08:00 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 22:18:20 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	clean_tools(t_tools *tools)
{
	close(tools->fd[1]);
	close(tools->fd[0]);
	reset_tools(tools);
	if (tools->exit_string)
		free(tools->exit_string);
	if (tools->env)
		ft_freetab(tools->env);
	clear_history();
}

/* A GOOD EXIT */
void	good_exit(t_tools *tools)
{
	clean_tools(tools);
	exit(0);
}

void	exit_with_code(t_tools *tools, int code)
{
	clean_tools(tools);
	exit(code);
}

// should only delete heredoc files if exiting the main process....
int	print_errno_exit(const char *arg, const char *errline, int custom_fail,
		t_tools *tools)
{
	ft_putstr_fd("msh: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errline)
		ft_putstr_fd(errline, 2);
	else
		ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	if (tools)
		clean_tools(tools);
	if (custom_fail > 0)
		exit(custom_fail);
	else
		exit(errno);
}
