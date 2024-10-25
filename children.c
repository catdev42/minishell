/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/25 12:20:28 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	add_child(pid_t pid, t_tools *tools)
{
	int	i;

	i = 0;
	while (i < MAXARGS)
	{
		if (tools->children[i] == 0)
			{tools->children[i] = pid;
            return 1;}
		i++;
        if i
	}
}

int	delete_child(pid_t pid, t_tools *tools)
{
	int	i;

	i = 0;
	while (i < MAXARGS)
	{
		if (tools->children[i] == pid)
		{
			if (kill(pid, 0))
			{
				ft_putstr_fd(2, "msh: error: the process is still alive!");
				error_exit(tools, 0);
			}
			tools->children[i] = 0;
			return (1);
		}
		i++;
	}
	ft_putstr_fd(2, "msh: pid not found in the array");
	return (0); // pid not found
}
