/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_reset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 19:22:37 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 18:54:42 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*removes the existing files after use*/
void	here_unlink(t_tools *tools)
{
	int	i;

	i = 0;
	while (i < MAXARGS)
	{
		if (access(tools->heredocs[i], F_OK) == -1)
		{
			if (errno == ENOENT)
				errno = 0;
			else
				print_error(tools->heredocs[i], "permission", NULL);
		}
		else
			unlink(tools->heredocs[i]);
		i++;
	}
	return ;
}

/* RESET TOOLS: leave env var and exit_string */
void	reset_tools(t_tools *tools)
{
	if (tools->tree)
		tree_free(tools->tree);
	if (tools->ln)
	{
		ft_bzero(tools->ln, tools->line_capacity);
		free_things(&tools->ln, NULL, NULL, -1);
	}
	tools->ln = NULL;
	if (tools->cl)
	{
		ft_bzero(tools->cl, tools->cl_capacity);
		free_things(&tools->cl, NULL, NULL, -1);
	}
	tools->tree = NULL;
	tools->cl = NULL;
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
	struct s_redircmd	*rcmd;
	struct s_pipecmd	*pcmd;

	pcmd = NULL;
	rcmd = NULL;
	if (node && node->type == EXEC)
		free((t_execcmd *)node);
	else if (node && node->type == REDIR)
	{
		rcmd = (struct s_redircmd *)node;
		tree_free(rcmd->cmd);
		free(rcmd);
	}
	else if (node && node->type == PIPE)
	{
		pcmd = (struct s_pipecmd *)node;
		tree_free(pcmd->left);
		tree_free(pcmd->right);
		free(pcmd);
	}
	return ;
}
