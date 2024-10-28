/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/28 13:09:28 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// /* RESET TOOLS: leave env var */

void	reset_tools(t_tools *tools)
{
	if (tools->line)
	{
		ft_bzero(tools->line, tools->line_capacity);
		free(tools->line);
	}
	tools->line = NULL;
	if (tools->cleanline)
	{
		ft_bzero(tools->cleanline, tools->cl_capacity);
		free(tools->cleanline);
	}
	if (tools->tree)
		tree_free(tools->tree);
	tools->tree = NULL;
	tools->cleanline = NULL;
	tools->cmd_end = NULL;
	tools->s = NULL;
	tools->lastpipe = NULL;
	tools->lastredir = NULL;
	// tools->p = NULL;
	// tools->p_next = NULL;
	tools->e_cline = NULL;
	tools->hereindex = 0;
	// TODO CLEAN HEREDOC FILES
}


/*removes the existing files after use*/
void	here_unlink(t_tools *tools)
{
	int	i;

	char(*heredocs)[20];
	heredocs = tools->heredocs;
	i = 0;
	while (i < MAXARGS)
	{
		if (access(heredocs[i], F_OK) == -1)
		{
			if (errno == ENOENT)
				errno = 0;
			else
				print_error(heredocs[i], "permission", NULL);
		}
		else
			unlink(heredocs[i]);
		i++;
	}
	return ;
}
void	tree_free(struct s_cmd *node)
{
	struct s_execcmd	*ecmd;
	struct s_redircmd	*rcmd;
	struct s_pipecmd	*pcmd;

	ecmd = NULL;
	pcmd = NULL;
	rcmd = NULL;
	if (node && node->type == EXEC)
	{
		ecmd = (struct s_execcmd *)node;
		free(ecmd);
	}
	else if (node && node->type == REDIR)
	{
		rcmd = (struct s_redircmd *)node;
		tree_free(rcmd->cmd);
	}
	else if (node && node->type == PIPE)
	{
		pcmd = (struct s_pipecmd *)node;
		tree_free(pcmd->left);
		tree_free(pcmd->right);
	}
	return ;
}

// static void	handle_exec(struct s_execcmd *cmd)
// {
// 	free(cmd);
// }

// static void	handle_redir(struct s_redircmd *cmd)
// {
// 	tree_free(cmd->cmd);
// }

// static void	handle_pipe(struct s_pipecmd *cmd)
// {
// 	tree_free(cmd->left);
// 	tree_free(cmd->right);
// }
