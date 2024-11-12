/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/11/11 16:34:02 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* DRAFT OF ENV: Unchecked */
int	env(char **argv, char **env, t_execcmd *ecmd, t_tools *tools)
{
	pid_t	pid;
	size_t	i;
	int		status;
	int		j;

	j = 0;
	if (get_matrix_len(argv) == 1)
		return (!print_tab(env));
	record_exit(0, tools);
	if (get_matrix_len(argv) > 1)
	{
		pid = fork();
		if (pid == 0)
		{
			signal_init_sa(tools->sa, SIG_DFL);
			i = 0;
			forked_image_env(tools, ecmd, i, j);
		}
		waitpid(pid, &status, 0);
		check_system_fail(status, tools, 0);
	}
	return (tools->exit_code);
}

void	forked_image_env(t_tools *tools, t_execcmd *ecmd, size_t i, size_t j)
{
	char	*equalsign;

	equalsign = NULL;
	while (ecmd->argv[++i])
	{
		equalsign = ft_strchr(ecmd->argv[i], '=');
		if (!equalsign)
			break ;
		equalsign[0] = 0;
		if (passchk(ecmd->argv[i], (long int)(equalsign - &ecmd->argv[i][0])))
			repl_or_app_var(ecmd->argv[i], &equalsign[1], tools->env, tools);
		equalsign[0] = '=';
	}
	if (!ecmd->argv[i] || !ecmd->argv[i][0])
		exit(!print_tab(tools->env));
	j = i--;
	while (ecmd->argv[++i])
		ecmd->argv[i - j] = ecmd->argv[i];
	while (i - j < MAXARGS)
		ecmd->argv[i++ - j] = NULL;
	handle_node((t_cmd *)ecmd, tools);
	clean_tools(tools);
	exit(0);
}

/* start is beginning of key and lim is its length = pointer = - to beginning*/
int	passchk(char *start, long int lim)
{
	long int	i;

	i = 0;
	if (lim == 0 || ft_isdigit(start[i]))
	{
		return (0);
	}
	while (i < lim)
	{
		if (ft_isspace(start[i]) || isquote(start[i])
			|| ft_strchr("*&|<>?{}()[]-+~°^§$#\%,.:;?!", start[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}
