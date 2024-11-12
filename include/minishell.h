/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:12:04 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/12 22:01:21 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
/**/
# include <readline/history.h>
# include <readline/readline.h>
/**/
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
/**/
# include "../libft/libft.h"
# include "structs.h"
/**/
# include "exits.h"
# include "parsing.h"

extern volatile sig_atomic_t	g_signal;

# define MIDLEN 256

# define UNEXP "syntax error near unexpected token"
# define UNCLOSED "unclosed quotes"
# define HEREPRINT "here-doc delimited by EOF, wanted"
# define ECHO "echo"
# define CD "cd"
# define PWD "pwd"
# define EXPORT "export"
# define UNSET "unset"
# define ENV "env"
# define EXIT "exit"

/* extra error codes */
# define SYSTEMFAIL 142
# define FORKFAIL 141
# define UNKNOWNERROR 143

/*TESTERS*/
void							walking(struct s_cmd *cmd);

/************************/
/********* EXEC *********/
/************************/

int								running_msh(t_tools *tools);
int								other_execution_type(t_tools *tool,
									t_execcmd *ecmd);
int								run_pipeless_builtin_tree(t_cmd *cmd,
									t_tools *tool);
int								run_redir(t_redircmd *rcmd, t_tools *tool);
void							run_pipe(t_pipecmd *pcmd, t_tools *tools);

int								builtin_check_walk(t_cmd *cmd);
int								is_builtin(char *s);
int								run_builtin(t_execcmd *cmd, t_tools *tool);

int								file_dir_noexist(const char *path,
									int fd_in_or_out);
int								check_file_type(t_redircmd *rcmd,
									int fd_in_or_out);
void							handle_node(t_cmd *cmd, t_tools *tool);
void							run_exec_node(t_tools *tool, t_execcmd *ecmd);
void							execute_execve(char *pathcmd, t_execcmd *ecmd,
									t_tools *tool);
char							*check_cmd_path(char *path, t_execcmd *cmd,
									t_tools *tools);

void							check_system_fail(int status, t_tools *tools,
									bool inmain);

/************************/
/******* BUILTINS.c ****/
/************************/
int								pwd(void);

int								env(char **argv, char **env, t_execcmd *ecmd,
									t_tools *tools);
int								passchk(char *start, long int lim);
void							forked_image_env(t_tools *tools,
									t_execcmd *ecmd, size_t i, size_t j);

int								ft_exit(t_execcmd *cmd, t_tools *tool);

int								unset(t_execcmd *cmd, t_tools *tool);

int								export(t_execcmd *cmd, t_tools *tool);
int								print_export(char **env);
int								repl_or_app_var(char *key, char *value,
									char **env, t_tools *tools);
int								export_check(char *arg, char *eqs,
									t_tools *tool);
void							put_var(char *key, char *value);

int								ft_strisnumeric(char *str);

int								append_var(char *key, char *value, char **env,
									t_tools *tools);

int								echo(t_execcmd *cmd);

int								cd(char **argv, char **env, t_tools *tools);

/************************/
/*******  ENV.C  ********/
/************************/
char							*get_var(char **env, char *var);
int								get_var_i(char **env, char *var);
int								copy_env(t_tools *tools, char **env);
char							*get_var_value(char **env, char *var);

/************************/
/******* SHELLinSHELL.C ********/
/************************/
int								check_mini(t_tools *tools);
void							change_shlvl(t_tools *tool);
void							exec_new_minishell(t_tools *tool,
									t_execcmd *ecmd);
int								fork_new_minishell(t_tools *tools);

/************************/
/******** MAIN.C ********/
/************************/
int								shell_loop(t_tools *tools);
void							new_line(void);

/************************/
/****** SIGNALS.C *******/
/************************/
void							signal_init_sa(struct sigaction *sa,
									void (*handler)(int));
void							handle_reprint_sig(int sig);
void							handle_printn_sig(int sig);
void							signal_init_sigquit(struct sigaction *sa);
// void	handle_recordonly_sig(int sig);
// void	handle_justexit_sig(int sig);

#endif
