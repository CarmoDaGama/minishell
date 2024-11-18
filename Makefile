NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
FT_PRINTF = ft_printf
SRCS_FILES = call_command.c \
	handle_signal.c \
	nodes_manager.c \
	search_command_in_path.c \
	tokens_manager.c \
	free_chars.c \
	parsing.c \
	parsing_error.c \
	handle_exit_status.c \
	parsing_helper.c \
	token_helper.c \
	handle_redirections.c \
	nodes_cleaning.c \
	parsing_utils.c \
	tokenizer.c \
	ft_echo.c \
	builtin.c \
	expanding.c \
	expand_helper.c \
	env_helper.c \
	builtin_helper.c \
	expanding_split.c \
	striping_quotes.c \
	change_directory.c \
	print_working_directory.c \
	enviroment.c \
	clean_all.c \
	tree_initializer.c \
	ft_export.c \
	ft_write_heredoc.c \
	ft_wait_pipe.c \
	ft_unset.c
	
OBJS_FILES = call_command.o \
	handle_signal.o \
	nodes_manager.o \
	search_command_in_path.o \
	tokens_manager.o \
	free_chars.o \
	parsing.o \
	parsing_error.o \
	handle_exit_status.o \
	parsing_helper.o \
	token_helper.o \
	handle_redirections.o \
	nodes_cleaning.o \
	parsing_utils.o \
	tokenizer.o \
	ft_wait_pipe.o \
	ft_echo.o \
	builtin.o \
	expanding.o \
	expand_helper.o \
	env_helper.o \
	builtin_helper.o \
	expanding_split.o \
	striping_quotes.o \
	change_directory.o \
	print_working_directory.o \
	enviroment.o \
	clean_all.o \
	tree_initializer.o \
	ft_export.o \
	ft_write_heredoc.o \
	ft_unset.o
	
all: $(NAME)

$(NAME): $(OBJS_FILES)
	make -C $(FT_PRINTF) 
	$(CC) $(CFLAGS) $(OBJS_FILES) -o $(NAME) minishell.c $(FT_PRINTF)/libftprintf.a -lreadline


clean:
	rm -f $(OBJS_FILES)
	make -C $(FT_PRINTF) fclean
	
fclean: clean
	rm -f minishell $(FT_PRINTF)/libftprintf.a

re: fclean all