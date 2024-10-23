/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:28 by spitul            #+#    #+#             */
/*   Updated: 2024/10/23 17:28:53 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	copy_env(t_tools *tools, char **env)
{
	char	**envp;
	int		len_pointers;
	int		i;
	char	**oldenv;
	int		len;

	oldenv = NULL; // checker if this was a previously allocated array
	if (tools->env == env)
		oldenv = tools->env;
	i = 0;
	len_pointers = 0;
	while (env[len_pointers] != NULL)
		len_pointers++;
	envp = ft_calloc((len_pointers + MAXARGS + 1), sizeof(char *));
	if (!envp)
		return (0);
	tools->env_len = len_pointers + MAXARGS;
	while (i < len_pointers && env[i])
	{
		len = MIDLEN;
		if (ft_strlen(env[i]) >= MIDLEN)
			len = ft_strlen(env[i]);
		envp[i] = safe_calloc(len + 2, sizeof(char), tools);
		if (!envp[i])
		{
			ft_freetab(envp);
			return (0); //fail
			/*changed to envp, we must delete currently allocated*/
		}
		if (!ft_strlcpy(envp[i], env[i], len))
			return (0); //fail
		i++;
	}
	if (oldenv)
		ft_freetab(oldenv);
	tools->env = envp;
	return (1); //success
}

// if (i == tools->env_len - 1)
// 	copy_env(tools, env)

/* Return the pointer to the variable definition or NULL if not found */
char	*get_var(char **env, char *var)
{
	int		i;
	size_t	len;
	char	*line;

	if (!env || !*env)
		return (NULL);
	line = NULL;
	i = 0;
	len = ft_strlen(var);
	while (env[i] && !line)
	{
		line = ft_strnstr(env[i], var, len);
		if (line && line[len] == '=')
			line = line + len + 1;
		else
			line = NULL;
		i++;
	}
	return (line);
}
