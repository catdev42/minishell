/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c      void	goodexit(tools)
{
	clean_tools(tools);
	exit(0);
}
										:+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/15 13:04:38 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* A GOOD EXIT */
void	goodexit(t_tools *tools)
{
	clean_tools(tools);
	exit(0);
}

/*
ERROR PATHS (please read): me
Parsing:
	if system error
		then exit (with cleaning)
	else
		return 0 in all functions (except
			-1 in mode finding (MOVE TO EXECUTION))
Execution (FORKS)
		Print error and exit with errno
		Waitpid catches the error, analyses to see if System Failure

		If CRITICAL ERROR
			(Progrvoid	goodexit(tools)
{
	clean_tools(tools);
	exit(0);
}
am should exit immediately as oppposed to just terminate one fork and keep going)
		else if just some error in a fork (like command not found)
			terminate just that fork, keep going with the program
*/

/* FOR MAIN PROCESS
Input NULL or what to print in the 3 prositions:
msh: (arg): (errline) `(errarg)'
If NULL at a specific position, that position is NOT printed
*/
int	print_error(const char *arg, const char *errline, const char *errarg)
{
	ft_putstr_fd("msh: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errline)
		ft_putstr_fd(errline, 2);
	// else
	// 	ft_putstr_fd(strerror(errno), 2);
	if (errarg)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(errarg, 2);
		ft_putstr_fd("\'", 2);
	}
	ft_putstr_fd("\n", 2);
	return (0);
}

/* FOR FORK PROCESvoid	goodexit(tools)
{
	clean_tools(tools);
	exit(0);
}
S
Input NULL if  a parameter is not needed.
//??? What do subsequent exit return is critical error???
ARG: input file name or command name
ERRLINE: custor error message or strerror(errnum) is printed
ERRARG: the thing inside of backticks if needed */
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
	// if (errarg)
	// {
	// 	ft_putstr_fd("`", 2);
	// 	ft_putstr_fd(errarg, 2);
	// 	ft_putstr_fd("\'", 2);
	// }
	ft_putstr_fd("\n", 2);
	if (tools)
		clean_tools(tools);
	if (custom_fail > 0)
		exit(custom_fail);
	else
		exit(errno);
}
/*ABOVE: work in progress - when do we exit errno and when do we just exit 1?*/

/*
FOR EXITING!
0: CTRL D or EXIT SUCCESS
1: malloc
3: just exit(1) nothing printed
*/
void	error_exit(t_tools *tools, int error)
{
	clean_tools(tools);
	clear_history();
	if (error == 0) // sucessful exit
		exit(0);
	else if (error >= 141 && error <= 143)
	{
		exit(error);
	}
	else if (error == 1)
		exit(errno);
	else if (error > 0)
	{
		// usually malloc error.... this need to be edited and replaces with errno exits...
		print_error(NULL, strerror(errno), NULL);
		exit(errno);
	}
	else
		exit(1);
}

void	clean_tools(t_tools *tools)
{
	reset_tools(tools);
	if (tools->env)
		ft_freetab(tools->env);
	// ft_memset(tools->heredocs, 0, sizeof(tools->heredocs));
		//its not allocated...
}

struct s_cmd	*clean_execs(struct s_cmd *first, struct s_cmd *second)
{
	struct s_redircmd	*rcmd;

	if (first)
	{
		if (first->type == REDIR)
		{
			rcmd = (struct s_redircmd *)first;
			free(rcmd);
		}
	}
	if (second)
	{
		if (first->type == REDIR)
		{
			rcmd = (struct s_redircmd *)second;
			free(rcmd);
		}
	}
	return (NULL);
}

/*
// ft_freetab in library

char	**free_array(char **res, int nb)
{
	int	i;

	i = 0;
	if (!res)
		return (NULL);
	while (i <= nb && res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (NULL);
}
*/
// void	print_errnum_error(const char *arg, const char *errline,
// 		const char *errarg, int err)
// {
// 	print_error(const char *arg, const char *errline, const char *errarg);
// 	if (err > 1)
// 		exit(errno);
// }
