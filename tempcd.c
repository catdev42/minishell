/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddavlety <ddavlety@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 13:34:30 by vketteni          #+#    #+#             */
/*   Updated: 2024/04/20 11:43:44 by ddavlety         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static char	*build_path(char *dir)
{
	char	*cwd;
	char	*path;
	char	*cwd_with_slash;

	cwd = getcwd(NULL, 0);
	cwd_with_slash = ft_strjoin(cwd, "/");
	free(cwd);
	if (!cwd_with_slash)
		return (NULL);
	path = ft_strjoin_free(cwd_with_slash, dir);
	return (path);
}

int	process_path(const char **argv, t_env *shell_env, char **ptr_path)
{
	if (!argv[1])
	{
		*ptr_path = get_variable_value("HOME", shell_env);
		if (!*ptr_path)
			return (builtin_err_gen("cd", "HOME not set", NULL));
	}
	else if (*argv[1] == '/')
		*ptr_path = ft_strdup((char *)argv[1]);
	else if (*argv[1] == '~')
		*ptr_path = ft_strjoin_free(get_variable_value("HOME", shell_env),
				&argv[1][1]);
	else
		*ptr_path = build_path((char *)argv[1]);
	return (0);
}

int	builtin_cd(const char **argv, t_env *shell_env)
{
	char	*path;
	char	*cwd;
	char	*value;

	if (argv[1] && argv[2])
		return (builtin_err_gen("cd", "too many arguments", NULL));
	process_path(argv, shell_env, &path);
	if (!path)
		return (builtin_err_gen("cd", "failed path", NULL));
	if (chdir(path))
	{
		write(2, "minishell: cd: ", 15);
		perror(argv[2]);
	}
	free(path);
	cwd = getcwd(NULL, 0);
	value = get_variable_value("PWD", shell_env);
	append_envp(shell_env, "OLDPWD", value);
	free(value);
	append_envp(shell_env, "PWD", cwd);
	free(cwd);
	return (0);
}
