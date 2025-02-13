/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:12:04 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/12 20:27:33 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXITS_H
# define EXITS_H

# include "./minishell.h"

/************************/
/******* ERROR.C ********/
/************************/
void			good_exit(t_tools *tools);
void			exit_with_code(t_tools *tools, int code);

int				print_errno_exit(const char *arg, const char *errline,
					int custom_fail, t_tools *tools);
void			error_exit_main(t_tools *tools, int error);
struct s_cmd	*clean_two(struct s_cmd *first, struct s_cmd *second);
void			clean_tools(t_tools *tools);
int				print_error(const char *arg, const char *arg2,
					const char *errline, const char *errarg);
/*char			**free_array(char **res, int nb);*/

/************************/
/******* CLEAN.C ********/
/************************/
void			here_unlink(t_tools *tools);
void			reset_tools(t_tools *tools);
void			tree_free(struct s_cmd *node);
void			free_things(char **s1, char **s2, char **s3, int fd);

#endif
