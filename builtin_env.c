/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/28 21:06:13 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* DRAFT OF ENV: Unchecked */
int	env(char **argv, char **env, t_execcmd *ecmd, t_tools *tools)
{
	pid_t	pid;
	size_t	i;
	char	*equalsign;
	int		status;
	int		j;

	equalsign = NULL;
	if (get_matrix_len(argv) == 1) // works
		return (!print_tab(env));  // ! cause printenv returns 0 on fail
	if (get_matrix_len(argv) > 1)
	{
		pid = fork();
		if (pid == 0)
		{
			init_sa(tools->sa, SIG_DFL);
			i = 1;
			while (ecmd->argv[i]) // assign env variables
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
			j = i;
			while (ecmd->argv[i])
			{
				ecmd->argv[i - j] = ecmd->argv[i];
				i++;
			}
			while (i - j < MAXARGS)
				argv[i++ - j] = NULL;
			handle_node((t_cmd *)ecmd, tools);
			clean_tools(tools);
			exit(0);
		}
		
		waitpid(pid, &status, 0);
		// usleep(500);
		check_system_fail(status, tools, 0);
	}
	return (1);
}

/* start is beginning of key and lim is its length = pointer = - to beginning*/
int	passcheck(char *start, long int lim)
{
	long int	i;

	i = 0;
	while (i < lim)
	{
		if (ft_isspace(start[i]) || isquote(start[i])
			|| ft_strchr("*&|<>?{}()[]", start[i]))
		{
			print_error("export", "not a valid identifier", start);
			return (0);
		}
		i++;
	}
	return (1);
}
