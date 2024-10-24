/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/24 19:00:25 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* DRAFT OF ENV: Unchecked */
int	env(char **argv, char **env, t_tools *tools)
{
	pid_t	pid;
	size_t	i;
	char	*equalsign;
	int		status;

	equalsign = NULL;
	if (get_matrix_len(argv) == 1)
		print_tab(env);
	if (get_matrix_len(argv) > 1)
	{
		pid = fork();
		if (pid == 0)
		{
			i = 0;
			while (argv[i]) // assign env variables
			{
				equalsign = ft_strchr(argv[i], '=');
				if (equalsign && passcheck(argv[i], (long int)(equalsign
							- &argv[i][0])))
				{
					equalsign[0] = 0; // nullterm to key
					replace_or_append_var(argv[i], &equalsign[1], env, tools);
					equalsign[0] = '='; // unnullterm
				}
				else
					break ;
				i++;
			}
			if (!argv[i] || !argv[i][0])
			{
				print_tab(tools->env);
				exit(0);
			}
            // line = ft_join(&argv[i])
            // struct s_execcmd* cmd; 
            argv =  &argv[i];
			tools->line = argv[i];
			clean_line(tools->line, ft_strlen(tools->line), tools);
			if (!tools->cleanline)
				print_errno_exit(NULL, NULL, errno, tools);
			if (!parseline(tools->cleanline, tools))
				exit(errno);
			running_msh(tools);
			clean_tools(tools);
			exit(0);
		}
		waitpid(pid, &status, 0);
		check_system_fail(status, tools);
	}
	return (1);
}

int	passcheck(char *start, long int lim)
{
	long int	i;

	i = 0;
	while (i < lim)
	{
		if (ft_isspace(start[i]) || isquote(start[i]) || ft_strchr("*&|<>?", start[i]))
			return (0);
		i++;
	}
	return (1);
}
