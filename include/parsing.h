/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:12:04 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 22:16:15 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/************************/
/******* INIT.C ********/
/************************/
struct s_cmd	*makeexec(void);
struct s_cmd	*makeredir(char *file, char *efile, bool append, int fd);
struct s_cmd	*makepipe(struct s_cmd *left, struct s_cmd *right);

/************************/
/***** HEREDOC.C ********/
/************************/
void			here_init(char heredocs[MAXARGS][MAXARGS], t_tools *tools);
int				createredir_here(char *delim, int mode, int fd, t_tools *tools);
char			*make_heredoc_fork(char *delim, t_tools *tools);
void			write_heredoc(int fd, char *alloc_delim, t_tools *tools);
char			*clean_line_expand_only(char *line, int linelen,
					t_tools *tools);
char			*make_actual_fork(pid_t pid, char *allo_delim, t_tools *tools,
					int fd);

/************************/
/******** LINE.C ********/
/************************/
int				copy_quotes(char *c_line, char *line, t_tools *tools);
int				copy_pipe(char *c_line, char *line, int current_line_index);
int				copy_redirect(char *c_line, char *line, int current_line_index);
char			*clean_line(char *line, int linelen, size_t i, t_tools *tools);
int				copy_spaces(char *c_line, char *line);
int				val_red(char *line);
// int				copy_var(char *c_line, char *line, t_tools *tools);
int				copy_var(char *c_line, char *line, t_tools *tools, int i);
int				remove_two(char *first, char *second);

/************************/
/******* PARSE.C ********/
/************************/
struct s_cmd	*parseline(char *cline, t_tools *tools);
int				create_pipe_node(struct s_cmd *left, struct s_cmd *right,
					t_tools *tools);
struct s_cmd	*createpipe(struct s_cmd *left, struct s_cmd *right,
					t_tools *tools);

/************************/
/**PARSER_REDIR_EXEC.C***/
/************************/
struct s_cmd	*parseexec(char *start, char *end_of_exec, t_tools *tools);
struct s_cmd	*parse_redirs(char *start, char *end_of_exec, t_tools *tools);
int				actually_parse_redir(int fd_in_or_out, char *start,
					t_tools *tools, bool append);
int				createredir(char *filestart, int mode, int fd, t_tools *tools);
struct s_cmd	*parseargv(char *start, char *end, t_tools *tools);

/************************/
/**** PARSE_UTILS.C *****/
/************************/
char			*peek(char *line, char *end, int token);
int				infile_or_outfile(char *start);
char			*get_token_end(char *namestart);
void			remove_useless_quotes_final(char *cline, size_t linecapacity);
void			nullify(char *cline, t_tools *tools);

/************************/
/******* UTILS.c ********/
/************************/
int				val_quts(char *line);
int				val_line(char *line);
int				istoken(char c);
int				isquote(char c);
int				isredir(char c);

/************************/
/******* UTILS2.c ********/
/************************/
char			*ft_join_one(char const *s1, char const *delim, char const *s2);
char			*safe_calloc(size_t nmemb, size_t size, t_tools *tools);
int				record_exit(int exit_num, t_tools *tools);
void			init_zero(size_t *i, size_t *j, char **str1, char **str2);

/************************/
/******* UTILS3.c ********/
/************************/
int				print_tab(char **envp);
int				get_matrix_len(char **matrix);
int				check_quotes(char *line, int i);
int				skip_quotes(char *line, int i);
int				skip_token(char *start, int i);

#endif