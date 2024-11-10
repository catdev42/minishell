/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 21:35:52 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/10 19:54:04 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

struct s_cmd	*makeexec(void)
{
	struct s_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((struct s_cmd *)cmd);
}
// redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)

struct s_cmd	*makeredir(char *file, char *efile, bool append, int fd)
{
	struct s_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (0);
	memset(cmd, 0, sizeof(*cmd));
	cmd->append = append;
	cmd->type = REDIR;
	cmd->cmd = NULL;
	cmd->file = file;
	cmd->efile = efile;
	// cmd->file = file;
	cmd->mode = -1;
	cmd->fd = fd;
	return ((struct s_cmd *)cmd);
}

struct s_cmd	*makepipe(struct s_cmd *left, struct s_cmd *right)
{
	struct s_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct s_cmd *)cmd);
}
