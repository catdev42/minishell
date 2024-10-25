/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/25 17:46:48 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"
#include <signal.h>

// volatile sig_atomic_t	global_signal = 0; // delete?

int	main(int argc, char **argv, char **env)
{
	t_tools	tools;

	if (argc > 1 || argv[1])
		ft_putstr_fd("This program does not accept arguments\n", 2);
	ft_memset(&tools, 0, sizeof(t_tools)); // init tools to zero
	here_init(tools.heredocs, &tools);
	copy_env(&tools, env);
	if (!tools.env || !tools.heredocs[0][0])
		(error_exit(&tools, 1));
	init_sa(&tools.sa);
	shell_loop(&tools);
	return (0);
}

int	shell_loop(t_tools *tools)
{
	while (1)
	{
		tools->sa.sa_handler = handle_signals;
		if (global_signal == SIGTERM) // TODO? or done
			break ;
		tools->line = readline("minishell: ");
		// global_signal = 0;
		if (!tools->line || global_signal == SIGTERM)
			ft_exit(NULL, tools);
		if (global_signal)
			tools->exit_code = global_signal + 128;
		tools->sa.sa_handler = SIG_DFL;
		/*TODO there has to be a way to call the exit function
		eithout a command struct
		*/
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
		ft_putstr_fd("  -- test of cleanline\n", 1);
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
	clear_history();
	// exit(tools->exit_code); //SUGGESTED TODO
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
