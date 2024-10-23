/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/23 19:10:48 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

volatile sig_atomic_t	global_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_tools	tools;

	// struct sigaction	sa;
	if (argc > 1 || argv[1])
		ft_putstr_fd("This program does not accept arguments\n", 2);
	ft_memset(&tools, 0, sizeof(t_tools)); // init tools to zero
	here_init(tools.heredocs, &tools);
	copy_env(&tools, env);
	if (!tools.env || !tools.heredocs[0][0])
		(error_exit(&tools, 1));
	// init_sa(&sa);
	shell_loop(&tools);
	print_tab(tools.env);
	clear_history();
	return (0);
}

int	shell_loop(t_tools *tools)
{
	while (1)
	{
		// if (global_signal == SIGTERM) // TODO? or done
		// 	break ;
		tools->line = readline("minishell: ");
		global_signal = 0;
		if (!valid_line(tools->line))
			continue ;
		add_history(tools->line);
		if (!valid_quotes(tools->line) || !valid_redirects(tools->line))
			continue ;
		tools->cleanline = clean_line(tools->line, ft_strlen(tools->line),
				tools);
		tools->e_cline = tools->cleanline + ft_strlen(tools->cleanline);
		if (!tools->cleanline)
			continue ;
		ft_putstr_fd(tools->cleanline, 1);
		ft_putstr_fd("\n", 1);
		if (!parseline(tools->cleanline, tools))
			continue ;
		// walking(tools->tree);
		// execution(tools->tree, tools);
		// if (global_signal == SIGTERM)
		// TODO? or done
		// 	break ;
		running_msh(tools);
		reset_tools(tools);
	}
	clean_tools(tools);
	return (0);
}

// CHECK IF THIS SHOULD BE A BUILTIN??? TODO TO DO
/* Liretally checks if exit was typed into the line as the first command */

/* OBSOLETE */
// void	checkexit(t_tools *tools)
// {
// 	if (!tools->line || (!strncmp(tools->line, "exit", 4)
// 			&& (ft_isspace(tools->line[5]) || tools->line[5] == 0)))
// 	{
// 		if (ft_strlen(tools->line) > 5)
// 			print_error("exit", "this command does not accept arguments", NULL);
// 			else
// 			error_exit(tools, 0);
// 	}
// }

void	new_line(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		new_line();
		global_signal = SIGINT;
	}
	else if (sig == SIGTERM)
		global_signal = SIGTERM;
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
