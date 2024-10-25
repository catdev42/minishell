/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 14:13:16 by spitul            #+#    #+#             */
/*   Updated: 2024/10/25 20:41:30 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int	echo(t_execcmd *cmd)
{
	int	i;
	int	cmp;

	if (!cmd)
		return (1); // exit fail
	if (!cmd->argv[1])
	{
		ft_putstr_fd("\n", 1);
		return (0);
	}
	cmp = ft_strncmp(cmd->argv[1], "-n", 3);
	if (cmp == 0)
		i = 2;
	else
		i = 1;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (cmp != 0)
		ft_putstr_fd("\n", 1);
	return (0); // exit success
}

// int cd(char *path) {
int	cd(char **argv, char **env, t_tools *tools)
{
	char	*buffer;

	buffer = NULL;
	buffer = safe_calloc(MIDLEN + 1, sizeof(char), tools);
	if (chdir(argv[1]) < 0)
	{
		print_error("cd", "cannot change directory to %s\n", NULL);
		return (1); // error
	}
	if (!replace_var("PWD", getcwd(buffer, MIDLEN), env)) /* if it errors*/
		return (1);
	free(buffer);
	return (0);
}

int	replace_var(char *key, char *value, char **env)
{
	int		i;
	char	*temp;
	char	*newvar;

	i = 1;
	if (!key)
		return (0);
	while (env[i])
	{
		/*if we find the var value*/
		if (get_var_value(env, key))
		{
			temp = env[i];
			newvar = ft_join_one(key, "=", value);
			if (!newvar)
				return (0);
			free(temp);
			env[i] = newvar;
		}
		i++;
	}
	return (1);
}

// reallocation is dest not provided
char	*ft_join_one(char const *s1, char const *delim, char const *s2)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*fullstr;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(delim) + ft_strlen(s2);
	fullstr = ft_calloc(len + 2, sizeof(char));
	if (!fullstr)
		return (NULL);
	j = 0;
	while (s1[j])
		fullstr[i++] = s1[j++];
	j = 0;
	while (delim[j])
		fullstr[i++] = delim[j++];
	j = 0;
	while (s2[j])
		fullstr[i++] = s2[j++];
	fullstr[i] = 0;
	return (fullstr);
}

// int	export(t_execcmd *cmd, t_tools *tool)
// {
// 	return (1);
// }

int	unset(t_execcmd *cmd, t_tools *tools)
{
	int		i;
	int		j;
	char	*temp;

	i = 1;
	if (!cmd)
		return (1);
	while (cmd->argv[i])
	{
		/*if we find the var value*/
		if (get_var(tools->env, cmd->argv[i]))
		{
			temp = tools->env[i];
			j = i;
			while (tools->env[j])
			{
				tools->env[j] = tools->env[j + 1];
				j++;
			}
			free(temp);
		}
		i++;
	}
	return (0);
}

int	ft_exit(t_execcmd *cmd, t_tools *tool)
{
	if (get_matrix_len(cmd->argv) > 1)
		print_error(NULL, "too many arguments", NULL);
	// maybe a little nonsensical
	// tree_free(tool->tree); //clean tools does this...
	clean_tools(tool);
	exit(0);
}

int	pwd(t_execcmd *cmd)
{
	char	*cwd;

	cwd = NULL;
	if (get_matrix_len(cmd->argv) > 1)
		ft_putstr_fd("pwd: too many arguments\n", 2);
	cwd = getcwd(NULL, 0); // should we check for malloc error
	if (cwd != NULL)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
		return (0);
	}
	else
		perror("pwd: error retrieving current directory:");
	return (1);
}

int	key_syntax_export(char *argv)
{
	int	i;

	if (!argv)
		return (0);
	i = 0;
	if (argv[0] == '=')
	{
		print_error("export", "not a valid identifier", argv);
		return (0);
	}
	while (argv[i] && argv[i] != '=')
	{
		if (!isalpha(argv[i]))
		{
			print_error("export", "not a valid identifier", argv);
			return (0);
		}
		i++;
	}
	return (1);
}

/* gets the variable name. checks the syntax of the var name
only if get key is not called for printing. syn_io is set to 0 for printing
1 for non-printing */
char	*get_key(char *arg, int syn_io)
{
	int		i;
	int		len;
	char	*key;

	if (!arg)
		return (NULL);
	if (syn_io == 1)
	{
		if (!key_syntax_export(arg))
			return (NULL);
	}
	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	len = i + 1;
	key = ft_calloc(len + 1, sizeof(char));
	if (!key)
		return (NULL);
	i = 0;
	while (i < len - 1)
	{
		key[i] = arg[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

int	print_export(char **env)
{
	int		i;
	char	*key;
	char	*value;

	if (!env)
		return (1);
	key = NULL;
	value = NULL;
	i = 0;
	while (env[i])
	{
		if (env[i][0] == '_')
		{
			i++;
			break ;
		}
		key = get_key(env[i], 0);
		if (!key)
			return (1);
		value = ft_strchr(env[i], '=');
		if (!value)
		{
			free(key);
			return (1);
		}
		value++;
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(key, 1);
		// ft_putstr_fd("=", 1);
		ft_putstr_fd("\"", 1);
		ft_putstr_fd(value, 1);
		ft_putstr_fd("\"\n", 1);
		i++;
	}
	return (0);
}

int	export(t_execcmd *cmd, t_tools *tool)
{
	int		i;
	char	*key;
	char	*value;
	if (!cmd || !cmd->argv[0])
		return (1);
	printf("argvlen %d\n", get_matrix_len(cmd->argv));
	if (get_matrix_len(cmd->argv) == 1)
		return (print_export(tool->env));
	i = 1;
	key = NULL;
	value = NULL;
	while (cmd->argv[i])
	{
		key = get_key(cmd->argv[i], 1);
		if (!key)
			return (1);
		value = ft_strchr(cmd->argv[i], '=');
		if (!value)
		{
			free(key);
			return (1);
		}
		value++;
		if (!replace_or_append_var(key, value, tool->env, tool))
		{
			free(key);
			return (1);
		}
		free(key); // key=NULL:
		i++;
	}
	return (0);
}
