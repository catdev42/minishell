/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 13:48:11 by myakoven         ###   ########.fr       */
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
	global_signal = sig;
}

/*for while in the rest of the program if something needs to be control C'd like a process killed*/
void	handle_printn_sig(int sig)
{
	if (sig == SIGINT)
		ft_putstr_fd("\n", 2);
	// ft_putstr_fd("I AM IN HANDLE_PRINTN_SIG", 2);
	global_signal = sig;
}

/**DO NOTHING EXCEPT SET GLOBAL */
void	handle_recordonly_sig(int sig)
{
	printf("I am in record only sig handler\n");
	global_signal = sig;
}
void	handle_justexit_sig(int sig)
{
	exit(sig);
}

// void	handle_printc_sig(int sig)
// {
// 	if (sig == SIGINT)

// 		ft_putstr_fd("\n", 2);

// 	global_signal = sig;
// }

/*have to test*/

/*
// void	init_sa_default(struct sigaction *sa)
// {
// 	sa->sa_handler = SIG_DFL;
// 	sigemptyset(&sa->sa_mask);
// 	sa->sa_flags = 0;
// 	if (sigaction(SIGINT, sa, NULL) == -1)
// 	{
// 		global_signal = SIGINT;
// 		perror("sigaction");
// 		exit(1);
// 	}
// }

// void	init_sa(struct sigaction *sa, )
// {
// 	sa->sa_handler = handle_signals;
// 	sigemptyset(&sa->sa_mask);
// 	sa->sa_flags = 0;
// 	if (sigaction(SIGINT, sa, NULL) == -1)
// 	{
// 		global_signal = SIGINT;
// 		perror("sigaction");
// 		exit(1);
// 	}
// }

// void	init_sa_heredoc(struct sigaction *sa)
// {
// 	sa->sa_handler = handle_here_signals;
// 	sigemptyset(&sa->sa_mask);
// 	sa->sa_flags = 0;
// 	if (sigaction(SIGINT, sa, NULL) == -1)
// 	{
// 		global_signal = SIGINT;
// 		perror("sigaction");
// 		exit(1);
// 	}
// }

// void	init_sa_fork(struct sigaction *sa)
// {
// 	sa->sa_handler = handle_here_signals;
// 	sigemptyset(&sa->sa_mask);
// 	sa->sa_flags = 0;
// 	if (sigaction(SIGINT, sa, NULL) == -1)
// 	{
// 		global_signal = SIGINT;
// 		perror("sigaction");
// 		exit(1);
// 	}
// }

// void init_sa_default(){

// }

// void	init_child_signals(void)
*/
