/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 20:25:45 by spitul            #+#    #+#             */
/*   Updated: 2024/11/12 16:44:09 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h" 

/*checks if here is a builtin command in a pipefree tree*/
int	builtin_check_walk(t_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	int					res;

	ecmd = NULL;
	rcmd = NULL;
	res = 0;
	if (cmd)
	{
		if (cmd->type == EXEC)
		{
			ecmd = (t_execcmd *)cmd;
			if (!ecmd->argv[0])
				return (0);
			res = is_builtin(ecmd->argv[0]);
			return (res);
		}
		else
		{
			rcmd = (t_redircmd *)cmd;
			res = builtin_check_walk(rcmd->cmd);
			return (res);
		}
	}
	return (res);
}

int	append_var(char *key, char *value, char **env, t_tools *tools)
{
	int		i;
	char	*newvar;

	newvar = NULL;
	i = 0;
	i = get_matrix_len(env);
	if (i >= tools->env_len - 1)
		copy_env(tools, env);
	newvar = ft_join_one(key, "=", value);
	if (!newvar)
		return (0);
	tools->env[i] = newvar;
	return (1);
}

int	repl_or_app_var(char *key, char *value, char **env, t_tools *tools)
{
	int		i;
	char	*newvar;
	char	*temp;

	temp = NULL;
	i = 0;
	if (!key)
		return (0);
	while (env[++i])
	{
		if (ft_strnstr(env[i], key, ft_strlen(key))
			&& env[i][ft_strlen(key)] == '=')
		{
			temp = env[i];
			break ;
		}
	}
	if (!temp)
		return (append_var(key, value, env, tools));
	newvar = ft_join_one(key, "=", value);
	if (!newvar)
		return (0);
	free(temp);
	env[i] = newvar;
	return (1);
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
