/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:53:30 by spitul            #+#    #+#             */
/*   Updated: 2024/11/11 22:31:21 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	print_cmd(t_execcmd *ecmd);
static void	print_redir(t_redircmd *rcmd);
static void	print_pipe(t_pipecmd *pcmd);

void	walking(struct s_cmd *cmd)
{
	struct s_redircmd	*rcmd;
	struct s_pipecmd	*pcmd;

	pcmd = NULL;
	rcmd = NULL;
	if (cmd)
	{
		if (cmd->type == 1)
		{
			print_cmd((t_execcmd *)cmd);
		}
		else if (cmd->type == 2)
		{
			rcmd = (t_redircmd *)cmd;
			print_redir(rcmd);
		}
		else if (cmd->type == 3)
		{
			pcmd = (t_pipecmd *)cmd;
			print_pipe(pcmd);
		}
		else
			return ;
	}
}

static void	print_cmd(t_execcmd *cmd)
{
	printf("EXEC node %d. \n", cmd->type);
	print_tab(cmd->argv);
	printf("ecmd: %p\n\n", cmd->argv);
}

static void	print_redir(t_redircmd *cmd)
{
	printf("REDIR cmd: %d\n", cmd->fd);
	printf("File name:%s. \n", cmd->file);
	walking(cmd->cmd);
}

static void	print_pipe(t_pipecmd *cmd)
{
	printf("PIPE node %d. \n\n", cmd->type);
	walking(cmd->left);
	walking(cmd->right);
}
