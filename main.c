/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/09 18:07:18 by myakoven         ###   ########.fr       */
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
		// pointer + len is address of end
		if (!tools->cleanline)
			continue ;
		printf("length of cleanline: %i\n", ft_strlen(tools->cleanline));
		// ft_putstr_fd(tools->cleanline, 1); //test cleanline
		// ft_putstr_fd("  -- test of cleanline\n", 1);
		if (!parseline(tools->cleanline, tools))
			continue ;
		if (ismini(tools->cleanline), tools)
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

int	execute_minishell(t_tools *tools)
{
	char	*end;
	int		fd;
	pid_t	pid;

	pid = -1;
	pid = fork();
	if (pid == -1)
		error_exit_main(tools, 1);
	init_sa(tools->sa, handle_recordonly_sig);
	if (pid == 0)
	{
		if (tools->tree->type == EXEC)
			exec_new_minishell(tools, (t_execcmd *)tools->tree);
		print_errno_exit(NULL, "This minishell does not handle this", 1, tools);
	}
	waitpid(pid, &tools->exit_code, 0);
	check_system_fail(tools->exit_code, tools, 1); 
	// we are in main and mimi doesnt get closed by sigint
	// if (global_signal == SIGINT)
	// {
	// here_unlink(tools);
	// close(fd);
	// return (NULL);
	// }
	init_sa(tools->sa, handle_printn_sig);
	record_exit(tools->exit_code, tools);
	return (0);
}

int	ismini(char *cleanline, t_tools *tools)
{
	int	successfullexit;

	successfullexit = -1;
	if (!ft_strncmp(cleanline, "minishell", 20) || !ft_strncmp(cleanline,
			"./minishell", 20))
	{
		execute_minishell(tools);
		return (1);
	}
}
