/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_reset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/28 20:50:03 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

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

/* RESET TOOLS: leave env var and exit_string */
void	reset_tools(t_tools *tools)
{
	if (tools->line)
	{
		ft_bzero(tools->line, tools->line_capacity);
		free_things(&tools->line, NULL, NULL, -1);
	}
	tools->line = NULL;
	if (tools->cleanline)
	{
		ft_bzero(tools->cleanline, tools->cl_capacity);
		free_things(&tools->cleanline, NULL, NULL, -1);
	}
	if (tools->tree)
		tree_free(tools->tree);
	tools->tree = NULL;
	tools->cleanline = NULL;
	tools->cmd_end = NULL;
	tools->s = NULL;
	tools->lastpipe = NULL;
	tools->lastredir = NULL;
	tools->e_cline = NULL;
	tools->hereindex = 0;
}

void	free_things(char **s1, char **s2, char **s3, int fd)
{
	if (s1 && *s1)
	{
		free(*s1);
		*s1 = NULL;
	}
	if (s2 && *s2)
	{
		free(*s2);
		*s2 = NULL;
	}
	if (s3 && *s3)
	{
		free(*s3);
		*s3 = NULL;
	}
	if (fd >= 0)
		close(fd);
}

/* part of reset */
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
