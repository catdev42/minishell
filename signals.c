/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 20:23:19 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	new_line(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
// init_sa(tool->sa, handle_reprint)
void	signal_init_sa(struct sigaction *sa, void (*handler)(int))
{
	sa->sa_handler = handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	if (sigaction(SIGINT, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}

/*for while in minishell: readline function
Provides a new empty line*/
void	handle_reprint_sig(int sig)
{
	if (sig == SIGINT)
		new_line();
	g_signal = sig;
}

/*for while in the rest of the program if something needs to be control C'd like a process killed*/
void	handle_printn_sig(int sig)
{
	if (sig == SIGINT)
		ft_putstr_fd("\n", 2);
	g_signal = sig;
}
