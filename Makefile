NAME	:= minishell

LIBNAME = libft.a
LIBDIR = ./libft
LIBFT = ./libft/libft.a

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#

HEADERS 	:= ./include
OBJDIR		:= ./obj
SRC     	:= builtins.c builtin_env.c builtins_utils.c clean.c env.c error.c exec_utils.c  exec_node_handling.c \
				exec.c init.c line.c linesyntax.c lineutils.c main.c \
					parse_heredoc.c parse_redir_exec.c parse_utils.c parse.c utils.c utils2.c utils3.c walking.c \
						shellinshell.c
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