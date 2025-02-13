/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:08:00 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/13 15:45:09 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* FOR MAIN PROCESS
msh: (arg): (errline) `(errarg)'
If input NULL at a specific position, that position is NOT printed
This always returns zero so it can be plces incide an error return */
int	print_error(const char *arg, const char *arg2, const char *errline,
		const char *errarg)
{
	ft_putstr_fd("msh: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg2 && arg2[0])
	{
		ft_putstr_fd(arg2, 2);
		ft_putstr_fd(": ", 2);
	}
	if (errline)
		ft_putstr_fd(errline, 2);
	if (errarg)
	{
		ft_putstr_fd(" `", 2);
		ft_putstr_fd(errarg, 2);
		ft_putstr_fd("\'", 2);
	}
	ft_putstr_fd("\n", 2);
	return (0);
}

// TODO should put malloc errors etc
void	good_exit_main(t_tools *tools)
{
	clean_tools(tools);
	here_unlink(tools);
	ft_memset(tools->heredocs, 0, sizeof(tools->heredocs));
	clear_history();
}

/*PUT "errno" in error field
UNLESS you specifically need to exit 1, in which case...put "1"
EXTRA (if you put -1 ot 0, will return errno as a catch all)
NO EXTI SUCCESS - USE GOOD_EXIT_MAIN
*/
/*NO SUCCESSFULL EXIT: -1 r 0 will exit with errno ***USE goodexit
NO PRINTING: use print_errno_exit */
/*ERROR 1 will print perror!*/
void	error_exit_main(t_tools *tools, int error)
{
	clean_tools(tools);
	here_unlink(tools);
	clear_history();
	if (error == -1 || error == 0)
		exit(errno);
	else if (error > 1)
		exit(error);
	else if (error == 1)
	{
		perror("msh");
		exit(1);
	}
	else
		exit(1);
}

/* For during parding allocations*/
struct s_cmd	*clean_two(struct s_cmd *first, struct s_cmd *second)
{
	if (first)
		tree_free(first);
	if (second)
		tree_free(second);
	return (NULL);
}
