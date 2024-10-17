/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:55:08 by spitul            #+#    #+#             */
/*   Updated: 2024/10/17 20:45:13 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*

THIS IS MY  CALL OF check_file_type and FAILURE checker...
	mode = check_file_type(start, fd_in_or_out, tools); // TODO !!!
	if (mode == -1) //because 0 is returned for O_RDONLY
		return (NULL); return out or exit fork
You can do this in your code cause it should work for builtin in the main process...
*/
/* Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0*/
int	file_dir_noexist(const char *path, int fd_in_or_out)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		// if it is an outfile and path is not found, we return 1
		// because a regular file will be created
		if (fd_in_or_out == 1 && errno == ENOENT)
			return (1); //not an error
		else
		{  //an error
			print_error(path, strerror(errno), NULL);
			return (0);
		}
	}
	if (S_ISREG(path_stat.st_mode))
	{
		return (1);
	}
	else if (S_ISDIR(path_stat.st_mode))
		return (2);
	else
		print_error(path, "Is neither a file nor a directory", NULL);
	return (0); //error
}

/* Return the MODE necessary for OPEN() file or dir */

int	check_file_type(t_redircmd *rcmd, int fd_in_or_out, t_tools *tools)
{
	char				*filepath;
	int					fileordir;

	if (!rcmd || fd_in_or_out < 0)
		return (0);
	fileordir = file_dir_noexist(rcmd->file, fd_in_or_out);
	if (fileordir == 0)
		return (-1);
	if (fileordir == 2 && rcmd->fd == 1) // directory, outfile 
		print_error(filepath, "Is a directory", NULL);
	if (fileordir == 1 && rcmd->append && rcmd->fd == 1) // reg file (not a directory), append, outfile
	//I HAVE TO ADDRESS APPEND IN REDIR CREATION (myakoven)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (fileordir == 1 && !rcmd->append && rcmd->fd == 1) //reg file (not a directory), trund, outfile
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (fileordir == 1 && rcmd->fd == 0) // reverted this to 1... cause regular file
		return (O_RDONLY);
	else if (fileordir == 2 && rcmd->fd == 0) // special condition for infile which is a directory
		return (O_RDONLY | __O_DIRECTORY);
	return (0);
}
