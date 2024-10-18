/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:45:09 by myakoven          #+#    #+#             */
/*   Updated: 2024/02/22 15:12:01 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	error_handler_exit(char *str)
{
	char	*firstw;

	firstw = first_word(str, ' ');
	if (!str)
		exit(1);
	if (is_spaces_only(str))
		ft_putstr_fd("pipex: No such file of directory: \n", 2);
	else
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(firstw, 2);
	}
	free(firstw);
	exit(1);
}

char	*first_word(char *cmd, char c)
{
	char	*arr;
	int		i;
	int		j;

	if (!cmd)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd[i] && cmd[i] == c)
		i++;
	while (cmd[i + j] && cmd[i + j] != c)
		j++;
	arr = ft_substr(&cmd[i], 0, j);
	if (!arr)
		return (NULL);
	if (!i && !j)
		arr = ft_strdup("");
	return (arr);
}

int	is_spaces_only(char *str)
{
	int	i;

	i = 0;
	if (str[0] == 0)
		return (0);
	while (str[i])
	{
		if (ft_isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*ft_jointhree(char *cmdpath, char const *s1, char const *s2,
		char const *s3)
{
	size_t	len_two;
	size_t	len_all;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	len_two = ft_strlen(s1) + ft_strlen(s2);
	len_all = len_two + ft_strlen(s3);
	while (i < ft_strlen(s1) && i < 1024)
	{
		cmdpath[i] = s1[i];
		i++;
	}
	while (i < len_two && i < 1024)
		cmdpath[i++] = s2[j++];
	j = 0;
	while (i < len_all && i < 1024)
		cmdpath[i++] = s3[j++];
	cmdpath[i] = 0;
	return (cmdpath);
}

int	pip_error(int err)
{
	if (err == 1)
	{
		ft_putstr_fd("Implementation: ./pipex infile 'cmd1' 'cmd2' outfile", 2);
		return (1);
	}
	return (0);
}
/*

void	ft_init_basics(int argc, char **argv, char **env, t_info *basics)
{
	basics->fd_in = open(argv[1], O_RDONLY);
	if (basics->fd_in < 0)
		// pip_error(2);
		error_handler_exit(argv[1], 1, basics);
	basics->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (basics->fd_in < 0 || basics->fd_out < 0)
		pip_error(2);
	basics->argc = argc;
	basics->argv = argv;
	basics->env = env;
}

replaced malloc with declared in main


char	*string;

string = malloc(sizeof(char) * len_all + 1);
if (!(string))
	return (NULL);
*/
