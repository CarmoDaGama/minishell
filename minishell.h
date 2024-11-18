/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:10:28 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/29 10:10:33 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <limits.h>
# include <errno.h>
# include <sys/stat.h>
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include "ft_printf/ft_printf.h"
# include <sys/types.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_tree_direction
{
	TD_LEFT,
	TD_RIGHT
}	t_tree_direction;
typedef enum e_branch_type
{
	N_PIPE,
	N_AND,
	N_OR,
	N_CMD,
}	t_branch_type;

typedef enum e_io_type
{
	IO_IN,
	IO_OUT,
	IO_HEREDOC,
	IO_APPEND
}	t_io_type;

typedef struct s_io_list
{
	t_io_type			type;
	char				*value;
	char				**expanded_value;
	int					here_doc[2];
	struct s_io_list	*prev;
	struct s_io_list	*next;
}	t_io_list;

typedef enum e_token_type
{
	T_IDENTIFIER,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_PIPE,
	T_O_PARENT,
	T_C_PARENT,
	T_AND,
	T_OR,
	T_NL,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_branch
{
	t_branch_type		type;
	t_io_list			*io_list;
	char				*args;
	char				**expanded_args;
	struct s_branch		*left;
	struct s_branch		*right;
}	t_branch;

typedef struct s_global_var
{
	char		*line;
	char		**env;
	char		*io_error;
	t_token		*tokens;
	t_token		*curr_token;
	t_branch	*ast;
	int			state_exit;
	bool		is_child;
	int			parse_err;
	t_env		*envlst;
}	t_global_var;

char			*ft_strjoin_auxi(char const *s1, char const *s2, char *c);
char			*mjn(char *s1, char *s2, char *set);
char			*t(char const *s1, char const *set);
void			read_command(t_global_var *global_var);
void			handle_newline(int sig);
int				get_exit_status(int status);
void			handle_in_process_son(int sig);
void			ft_write_heredoc(t_branch *branch, t_global_var *global_var);
int				run_child_pipe(t_branch *branch, int *pfds,
					t_tree_direction direction, t_global_var *global_var);
int				print_working_directory(void);
int				change_directory(char **cd, t_global_var *global_var);
t_token			*ft_tokenize(t_global_var *global_var);
int				ft_append_separator(t_token_type type,
					char **line_ptr, t_token **token_list);
int				ft_append_identifier(char **line_ptr,
					t_token **token_list, t_global_var *global_var);
int				ft_handle_separator(char **line_ptr, t_token **token_list);
t_token			*ft_tokenization_handler(t_global_var *global_var);
t_token			*ft_new_token(char *value, t_token_type type);
void			ft_token_list_add_back(t_token **lst, t_token *new_token);
void			ft_clear_token_list(t_token **lst);
int				ft_is_quote(char c);
int				ft_is_separator(char *s);
void			ft_skip_spaces(char **line);
bool			ft_skip_quotes(char *line, size_t *i);
void			ft_print_quote_err(char c, t_global_var *global_var);
t_branch		*get_term(t_global_var *global_var);
t_branch		*get_sequence(int min_prec, t_global_var *global_var);
t_branch		*join_branch(t_token_type type_op, t_branch *left,
					t_branch *right, t_global_var *global_var);
t_branch		*parsing(t_global_var *global_var);
void			ft_get_next_token(t_global_var *global_var);
bool			ft_curr_token_is_binop(t_global_var *global_var);
int				ft_prec(t_token_type type);
int				ft_curr_token_prec(t_global_var *global_var);
t_branch_type	ft_get_branch_type(t_token_type type);
t_io_type		ft_get_io_type(t_token_type type);
t_branch		*ft_new_branch(t_branch_type type);
t_io_list		*ft_new_io_list(t_token_type type, char *value);
void			ft_append_io_list(t_io_list **lst, t_io_list *new);
void			ft_clear_io_list(t_io_list **lst);
void			ft_clear_cmd_branch(t_branch *node);
void			ft_recursive_clear_ast(t_branch *node);
void			ft_clear_ast(t_branch **ast, t_global_var *global_var);
bool			ft_get_io_list(t_io_list **io_list, t_global_var *global_var);
bool			ft_join_args(char **args, t_global_var *global_var);
t_branch		*ft_get_simple_cmd(t_global_var *global_var);
void			ft_free_char2(char **tofree);
void			ft_free_char3(char ***tofree);
void			ft_set_parse_err(int type, t_global_var *global_var);
void			ft_handle_parse_err(t_global_var *global_var);
bool			ft_is_redir(t_token_type type);
char			*ft_strjoin_with(char const *s1, char const *s2, char c);
const char		*token_type_to_string(t_token_type type);
int				search_command_in_path(char **command,
					t_global_var *global_var);
void			main_signal(t_global_var *global_var);
void			print_readline(void);
void			opcions_command(t_branch *branch, t_global_var *global_var);
void			call_command(t_branch *branch, t_global_var *global_var);
void			handle_exit_status( t_branch *branch, t_global_var *global_var);
int				handle_redirections(t_branch *branch, t_global_var *global_var);
const char		*oken_type_to_string(t_token_type type);
void			print_branch(t_branch *node);
void			print_io_list(t_io_list *io_list);
void			print_t_branch(t_branch *node);
void			ft_recursive_print_ast(t_branch *node);
void			print_token(t_token *token);
int				ft_echo(char **args);
void			ft_exit(char **args, t_global_var *global_var);
bool			is_builtin(char *arg);
int				run_builtin(char **args, t_global_var *global_var);
char			*ft_strjoin_f(char *s1, char *s2);
char			*ft_handle_dollar(char *str, size_t *i,
					t_global_var *global_var);
char			**expanding(char *s, t_global_var *global_var);
bool			ft_is_valid_var_char(char c);
char			*ft_handle_normal_str(char *str, size_t *i);
char			*ft_handle_squotes(char *str, size_t *i);
char			*ft_handle_dquotes(char *str, size_t *i,
					t_global_var *global_var);
bool			ft_env_entry_exists(char *key, t_global_var *global_var);
char			*ft_get_envlst_val(char *key, t_global_var *global_var);
void			ft_envlst_back(t_env *new, t_global_var *global_var);
void			ft_update_envlst(char *key, char *value,
					bool create, t_global_var *global_var);
void			*manage_momery(void *ptr, bool clean);
void			*mm(void *var, bool clean);
bool			ft_is_delimiter(char *delimiter, char *str);
char			*ft_clean_empty_strs(char *str);
char			**expanding_split(char const *s);
char			*striping_quotes(char *str);
char			*ft_extract_key(char *str);
char			*ft_extract_value(char *str);
void			ft_init_envlst(t_global_var *global_var);
int				ft_env(t_global_var *global_var);
void			clean_all(t_global_var *global_var);
void			init_end_branch(t_branch *branch, t_global_var *global_var);
int				ft_export(char **argv, t_global_var *global_var);
int				ft_check_key(char *str);
bool			ft_unset(char **args, t_global_var *global_var);
void			heredoc_expanding(char *str, int fd, t_global_var *global_var);
char			*wait_pipe(char *line, t_global_var *global_var);
bool			is_numeric(char *str);
#endif
