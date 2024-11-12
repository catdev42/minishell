/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 20:00:21 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 20:29:20 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H
# include "./minishell.h"

# define MAXARGS 20
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define ALPHA 4

typedef struct s_cmd
{
	int						type;
}							t_cmd;

typedef struct s_execcmd
{
	int						type;
	char					*argv[MAXARGS];
	char					*eargv[MAXARGS];
}							t_execcmd;

typedef struct s_redircmd
{
	int						type;
	bool					append;
	struct s_cmd			*cmd;
	char					*file;
	char					*efile;
	int						mode;
	int						fd;
}							t_redircmd;

typedef struct s_pipecmd
{
	int						type;
	struct s_cmd			*left;
	struct s_cmd			*right;
}							t_pipecmd;

typedef struct s_tools
{
	struct sigaction		*sa;
	int						fd[2];
	char					**env;
	int						env_len;
	char					*ln;
	size_t					line_capacity;
	char					*cl;
	char					*e_cline;
	size_t					cl_capacity;
	int						exit_code;
	char					*exit_string;
	struct s_cmd			*tree;

	char					*s;
	char					*cmd_end;
	char					heredocs[MAXARGS][MAXARGS];
	int						hereindex;
	struct s_pipecmd		*lastpipe;
	struct s_redircmd		*lastredir;
}							t_tools;

#endif

/*
// //one for each command with its own redirects and argc
// typedef struct s_parsed_command
// {
// 	char		*line;
// 	int			argc;
// 	char		**argv;
// 	char		**redirects;
// 	t_token		*redir_tokens;
// }				t_parsed;
// typedef struct line_saver
// {
// 	char	**line_pointers;
// 	int		*line_lengths;
// }

// enum				e_tokentype
// {
// 	NONE,
// 	PIPE,
// 	INPUT,
// 	HEREDOC,
// 	OUTFILE,
// 	OUTFILE_APPEND,
// };
*/