/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/26 19:21:36 by myakoven         ###   ########.fr       */
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
	// ft_putstr_fd("I am in handle_signals", 2);
	if (sig == SIGINT)
	{
		new_line();
		global_signal = SIGINT;
	}
	else if (sig == SIGTERM)
		global_signal = SIGTERM;
	else
		global_signal = sig;
}

void	handle_here_signals(int sig)
{
	if (sig == SIGINT)
	{
		new_line();
		global_signal = SIGINT;
	}
	else
		global_signal = sig;
	// ft_putstr_fd("I am in handle_here_signals", 2);
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

void	init_sa_heredoc(struct sigaction *sa)
{
	sa->sa_handler = handle_here_signals;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	if (sigaction(SIGINT, sa, NULL) == -1)
	{
		global_signal = SIGINT;
		perror("sigaction");
		exit(1);
	}
}

void	init_sa_default(struct sigaction *sa)
{
	sa->sa_handler = SIG_DFL;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	if (sigaction(SIGINT, sa, NULL) == -1)
	{
		global_signal = SIGINT;
		perror("sigaction");
		exit(1);
	}
}



// void init_sa_default(){

// }

// void	init_child_signals(void)
