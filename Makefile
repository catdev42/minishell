NAME	:= minishell

LIBNAME = libft.a
LIBDIR = ./libft
LIBFT = ./libft/libft.a

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#

HEADERS 	:= ./include
OBJDIR		:= ./obj
SRC     	:= builtin_env.c builtins_utils.c builtins.c builtins2.c\
				clean_reset.c env.c exec_node_handling.c exec_utils.c exec.c \
					exit_fork.c exit_main.c init.c line.c linesyntax.c lineutils.c \
						main.c parse_heredoc.c parse_redir_exec.c parse_utils.c parse.c \
							shellinshell.c signals.c utils.c utils2.c utils3.c \
								walking.c  #tester
# exec_utils2.c  execredir.c
OBJS     	:= $(addprefix $(OBJDIR)/,$(SRC:.c=.o))

CC      	:= cc 
CFLAGS  	:= -Wall -Wextra -Werror -g -fPIC -I/usr/local/opt/readline/include
LDFLAGS 	:= -L/usr/local/opt/readline/lib -lreadline

#------------------------------------------------#
#   RECIPES                                      #
#------------------------------------------------#
# .SILENT:

all: $(NAME)

$(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $< -I$(HEADERS)

$(LIBFT): runlibft

runlibft:
	$(MAKE) -C $(LIBDIR)


$(NAME): $(LIBFT) $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME) 

clean:
	rm -f $(OBJS)
	make -C $(LIBDIR) clean

fclean: clean
	rm -rf $(OBJDIR)
	rm -f $(NAME)
	make -C $(LIBDIR) fclean


re: fclean all

.PHONY: all clean fclean re