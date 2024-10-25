/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/25 12:31:20 by myakoven         ###   ########.fr       */
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

void	handle_signals(int sig)
{
    global_signal = sig;
	// if (sig == SIGINT)
	// {
	// 	// new_line();
	// 	global_signal = SIGINT;
	// }
	// else if (sig == SIGTERM)
	// 	global_signal = SIGTERM;
}


void	init_sa(struct sigaction *sa)
{
	sa->sa_handler = handle_signals;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	if (sigaction(SIGINT, sa, NULL) == -1)
	{
		global_signal = SIGINT;
		perror("sigaction");
		exit(1);
	}
}
