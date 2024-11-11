/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_good.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 15:59:02 by myakoven         ###   ########.fr       */
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
	tools.sa = &sa;
	signal_init_sa(tools.sa, handle_reprint_sig);
	here_init(tools.heredocs, &tools);
	copy_env(&tools, env);
	if (!tools.env || !tools.heredocs[0][0])
		(error_exit_main(&tools, 1));
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
		signal_init_sa(tools->sa, handle_reprint_sig);
		dup2(fd[0], 0);
		dup2(fd[1], 1);
		here_unlink(tools);
		reset_tools(tools);
		if (g_signal == SIGTERM)
			break ;
		g_signal = 0;
		tools->ln = readline("minishell: ");
		signal_init_sa(tools->sa, handle_printn_sig);
		if (!tools->ln || g_signal == SIGTERM)
			ft_exit(NULL, tools);
		if (g_signal)
			record_exit(g_signal + 128, tools);
		if (!val_line(tools->ln))
			continue ;
		add_history(tools->ln);
		if (!val_quts(tools->ln) || !val_red(tools->ln))
			continue ;
		tools->cl = clean_line(tools->ln, ft_strlen(tools->ln), tools);
		tools->e_cline = tools->cl + ft_strlen(tools->cl);
		if (!tools->cl)
			continue ;
		if (!parseline(tools->cl, tools))
			continue ;
		if (tools->tree->type == EXEC && ((t_execcmd *)tools->tree)->argv[0]
			&& !ft_strncmp(((t_execcmd *)tools->tree)->argv[0], "./minishell",
				12))
		{
			fork_new_minishell(tools);
			continue ;
		}
		running_msh(tools);
	}
	close(fd[1]);
	close(fd[0]);
	clean_tools(tools);
	clear_history();
	return (0);
}
// walking(tools->tree); //test tree

/*reference: heredoc execution*/
/*should return 0 on some sort of failure, */
int	fork_new_minishell(t_tools *tools)
{
	pid_t	pid;

	pid = -1;
	signal_init_sa(tools->sa, SIG_IGN);
	pid = fork();
	if (pid == -1)
		error_exit_main(tools, 1);
	if (pid == 0)
	{
		// init_sa(tools->sa, handle_reprint_sig);
		if (tools->tree->type == EXEC)
			exec_new_minishell(tools, (t_execcmd *)tools->tree);
		printf("Am i still in this child process?\n");
		print_errno_exit(NULL, "This msh does not handle this", 1, tools);
	}
	waitpid(pid, &tools->exit_code, 0);
	// printf("I exited child now");
	check_system_fail(tools->exit_code, tools, 1);
	// we are in main and mini doesnt get closed by sigint
	record_exit(tools->exit_code, tools);
	return (tools->exit_code);
}

// /*should this return */
// int	ismini(char *cl, t_tools *tools)
// {
// 	int	is_exit_bad;

// 	is_exit_bad = 0;
// 	if (!ft_strncmp(cl, "minishell", 20) || !ft_strncmp(cl,
// 			"./minishell", 20))
// 		is_exit_bad = fork_new_minishell(tools);
// 	// if (is_exit_good = )
// 	if (is_exit_bad)
// 		return (0);
// 	return (1);
// }
