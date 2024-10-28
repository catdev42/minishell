/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 19:16:34 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/28 14:01:54 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"


int	infile_or_outfile(char *start)
{
	if (*start == '<')
		return (0);
	else if (*start == '>')
		return (1);
	return (-1);
}


// /* Allocates a filename/path. MUST BE FREED */
// char	*get_redir_path(char *redir, t_tools *tools)
// {
// 	int		i;
// 	char	*filename;
// 	int		start;

// 	i = 0;
// 	while (redir[i] && isredir(redir[i]))
// 	{
// 		i++;
// 	}
// 	start = i;
// 	while (redir[i] && !ft_isspace(redir[i]))
// 	{
// 		if (isquote(redir[i]))
// 			i = skip_quotes(redir, i);
// 		i++;
// 	}
// 	filename = ft_substr(redir, start, i - start);
// 	if (!filename)
// 		error_exit_main(tools, 1);
// 	strip_quotes_final(filename);
// 	return (filename);
// }

/* Gets pointer to the space after the token */
char	*get_token_end(char *namestart)
{
	int	i;

	i = 0;
	while (namestart[i] && !ft_isspace(namestart[i]))
	{
		if (isquote(namestart[i]))
			i = skip_quotes(namestart, i);
		i++;
	}
	return (&namestart[i]);
}


// /* Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0*/
// int	file_dir_noexist(const char *path, int fd_in_or_out)
// {
// 	struct stat	path_stat;

// 	if (stat(path, &path_stat) != 0)
// 	{
// 		if (errno == ENOENT && fd_in_or_out == 1)
// 			return (1);
// 		/* this should be checked only with infiles*/
// 		print_error(path, strerror(errno), NULL);
// 		return (0);
// 	}
// 	if (S_ISREG(path_stat.st_mode))
// 	{
// 		return (1);
// 	}
// 	else if (S_ISDIR(path_stat.st_mode))
// 		return (2);
// 	else
// 		print_error(path, "Is neither a file nor a directory", NULL);
// 	return (0);
// }


/* Return the MODE necessary for OPEN() file or dir */
/*
int	check_file_type(char *start, int fd_in_or_out, t_tools *tools)
{
	char	*filepath;
	int		fileordir;

	if (!start || fd_in_or_out < 0)
		return (0);
	filepath = get_redir_path(start, tools);
	fileordir = file_dir_noexist(filepath, fd_in_or_out);
	if (fileordir == 0)
	{
		return (-1);
	}
	if (start[0] == '>' && fileordir == 2)
		print_error(filepath, "Is a directory", NULL);
	free(filepath);
	if (fileordir == 1 && start[0] == '>' && start[1] == '>')
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (fileordir == 1 && start[0] == '>')
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (fileordir == 1 && start[0] == '<')
		return (O_RDONLY);
	else if (fileordir == 2 && start[0] == '<')
		return (O_RDONLY | __O_DIRECTORY);
	return (0);
	// else if (start[0] == '<' &&start[1] == '<')
	// 		; //HEREDOC?
}
*/