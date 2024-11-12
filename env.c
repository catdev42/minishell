/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:07:28 by spitul            #+#    #+#             */
/*   Updated: 2024/11/11 22:40:33 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static int	copy_env_helper(char **envp, char **env, int i)
{
	int	len;

	len = MIDLEN;
	if (ft_strlen(env[i]) >= MIDLEN)
		len = ft_strlen(env[i]);
	envp[i] = ft_calloc(len + 2, sizeof(char));
	if (!envp[i])
	{
		ft_freetab(envp);
		return (0);
	}
	if (!ft_strlcpy(envp[i], env[i], len))
		return (0);
	return (1);
}

int	copy_env(t_tools *tools, char **env)
{
	char	**envp;
	int		len_pointers;
	int		i;
	char	**oldenv;

	oldenv = NULL;
	if (tools->env == env)
		oldenv = tools->env;
	len_pointers = get_matrix_len(env);
	envp = ft_calloc((len_pointers + MAXARGS + 1), sizeof(char *));
	if (!envp)
		return (0);
	tools->env_len = len_pointers + MAXARGS;
	i = 0;
	while (i < len_pointers && env[i])
	{
		if (!copy_env_helper(envp, env, i))
			return (0);
		i++;
	}
	if (oldenv)
		ft_freetab(oldenv);
	tools->env = envp;
	return (1);
}

/* Return the pointer to the variable value or NULL if not found */
char	*get_var_value(char **env, char *var)
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
		if (line && len < ft_strlen(line) && line[len] == '=')
			return (line + len + 1);
		line = NULL;
		i++;
	}
	return (line);
}

/* Return the pointer to the variable definition or NULL if not found */
char	*get_var(char **env, char *var)
{
	int		i;
	size_t	len;
	char	*line;

	if (!env || !*env || !var)
		return (NULL);
	line = NULL;
	i = 0;
	len = ft_strlen(var);
	while (env[i] && !line)
	{
		line = ft_strnstr(env[i], var, len);
		if (line && line[len] == '=')
			return (line);
		line = NULL;
		i++;
	}
	return (NULL);
}

/* returns the index of the found variable or -1 */
int	get_var_i(char **env, char *var)
{
	int		i;
	size_t	len;
	char	*line;

	if (!env || !*env || !var)
		return (-1);
	line = NULL;
	i = 0;
	len = ft_strlen(var);
	while (env[i] && !line)
	{
		line = ft_strnstr(env[i], var, len);
		if (line && line[len] == '=')
			return (i);
		line = NULL;
		i++;
	}
	return (-1);
}
