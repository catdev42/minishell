/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/28 21:15:05 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

volatile sig_atomic_t	global_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_tools				tools;
	struct sigaction	sa;

	if (argc > 1 || argv[1])
		ft_putstr_fd("This program does not accept arguments\n", 2);
	ft_memset(&tools, 0, sizeof(t_tools));
	tools.sa = &sa;
	here_init(tools.heredocs, &tools);
	copy_env(&tools, env);
	if (!tools.env || !tools.heredocs[0][0])
		(error_exit_main(&tools, 1));
	init_sa(tools.sa, handle_reprint_sig);
	shell_loop(&tools);
	return (0);
}

int	shell_loop(t_tools *tools)
{
	int	fd[2];

	fd[1] = dup(1);
	fd[0] = dup(0);
	while (1)
	{
		dup2(fd[0], 0);
		dup2(fd[1], 1);
		here_unlink(tools);
		reset_tools(tools);
		init_sa(tools->sa, handle_reprint_sig);
		if (global_signal == SIGTERM)
			break ;
		global_signal = 0;
		tools->line = readline("minishell: ");
		// if (peek(PIPE))
		// 	fork()	
		// 	parse in fork
		// else if (! first alpha word is a builting)
		// 	fork
		// 	else run buitlin
		init_sa(tools->sa, handle_printn_sig);
		if (!tools->line || global_signal == SIGTERM)
			ft_exit(NULL, tools);
		if (global_signal)
			record_exit(global_signal + 128, tools);
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
		// ft_putstr_fd(tools->cleanline, 1); //test cleanline
		// ft_putstr_fd("  -- test of cleanline\n", 1);
		if (!parseline(tools->cleanline, tools))
			continue ;
		// walking(tools->tree); //test tree
		running_msh(tools);
	}
	close(fd[1]);
	close(fd[0]);
	clean_tools(tools);
	clear_history();
	return (0);
}
