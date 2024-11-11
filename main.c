/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 20:22:24 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_tools				tools;
	struct sigaction	sa;

	if (argc > 1 || argv[1])
		ft_putstr_fd("This program does not accept arguments\n", 2);
	ft_memset(&tools, 0, sizeof(t_tools));
	record_exit(0, &tools);
	tools.fd[1] = dup(1);
	tools.fd[0] = dup(0);
	tools.sa = &sa;
	signal_init_sa(tools.sa, handle_reprint_sig);
	here_init(tools.heredocs, &tools);
	copy_env(&tools, env);
	if (!tools.env || !tools.heredocs[0][0])
		(error_exit_main(&tools, 1));
	shell_loop(&tools);
	here_unlink(&tools);
	clean_tools(&tools);
	clear_history();
	return (0);
}

int	shell_loop(t_tools *tool)
{
	while (1)
	{
		signal_init_sa(tool->sa, handle_reprint_sig);
		dup2(tool->fd[0], 0);
		dup2(tool->fd[1], 1);
		here_unlink(tool);
		reset_tools(tool);
		g_signal = 0;
		tool->ln = readline("minishell: ");
		signal_init_sa(tool->sa, handle_printn_sig);
		if (!tool->ln || g_signal == SIGTERM)
			ft_exit(NULL, tool);
		if (g_signal)
			record_exit(g_signal + 128, tool);
		if (val_line(tool->ln))
			add_history(tool->ln);
		if (!val_line(tool->ln) || !val_quts(tool->ln) || !val_red(tool->ln))
			continue ;
		tool->cl = clean_line(tool->ln, ft_strlen(tool->ln), tool);
		tool->e_cline = tool->cl + ft_strlen(tool->cl);
		if (!tool->cl || !parseline(tool->cl, tool) || check_mini(tool))
			continue ;
		running_msh(tool);
	}
	return (0);
}
