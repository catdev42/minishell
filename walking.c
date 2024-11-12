/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:53:30 by spitul            #+#    #+#             */
/*   Updated: 2024/10/10 00:43:29 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static void	print_cmd(t_execcmd *ecmd);
static void	print_redir(t_redircmd *rcmd);
static void	print_pipe(t_pipecmd *pcmd);

void		walking(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	struct s_pipecmd	*pcmd;

	ecmd = NULL;
	pcmd = NULL;
	rcmd = NULL;
	if (cmd)
	{
		if (cmd->type == 1)
		{
			ecmd = (t_execcmd *)cmd;
			print_cmd(ecmd);
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

// void	walking(t_cmd *cmd)
// {
// 	struct s_execcmd	*ecmd;
// 	struct s_redircmd	*rcmd;
// 	struct s_pipecmd	*pcmd;

// 	// ft_memset((void *)ecmd, 0, sizeof(*ecmd));
// 	// ft_memset((void *)rcmd, 0, sizeof(*rcmd));
// 	// ft_memset((void *)pcmd, 0, sizeof(*pcmd));
// 	ecmd = NULL;
// 	pcmd = NULL;
// 	rcmd = NULL;
// 	*i = 1;
// 	if (cmd)
// 	{
// 		if (cmd->type == 1)
// 		{
// 			ecmd = (t_execcmd *)cmd;
// 			print_cmd(ecmd);
// 		}
// 		else if (cmd->type == 2)
// 		{
// 			rcmd = (t_redircmd *)cmd;
// 			print_redir(rcmd);
// 		}
// 		else if (cmd->type == 3)
// 		{
// 			pcmd = (t_pipecmd *)cmd;
// 			print_pipe(pcmd);
// 		}
// 		else
// 			exit(1); // where is this returned and what happens to it
// 	}
// }

// void	print_cmd(t_execcmd *ecmd)
// {
// 	printf("node %d. \n", *i);
// 	printf("we are in an exec struct\n");
// 	printf("ecmd: %p\n\n", ecmd->argv);
// 	(*i)++;
// }
// void	print_redir(t_redircmd *rcmd)
// {
// 	printf("node %d. \n", *i);
// 	printf("redir cmd: %d\n", rcmd->fd);
// 	(*i)++;
// 	walking(rcmd->cmd);
// }

// void	print_pipe(t_pipecmd *pcmd)
// {
// 	printf("node %d. \n", *i);
// 	printf("pipe cmd\n");
// 	(*i)++;
// 	walking(pcmd->left);
// 	walking(pcmd->right);
// }
