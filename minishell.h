#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include <termios.h>
#include <limits.h>
#include "ft_printf/ft_printf.h"

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef enum e_err_msg 
{
    ERRMSG_CMD_NOT_FOUND,
    ERRMSG_NO_SUCH_FILE,
    ERRMSG_PERM_DENIED,
    ERRMSG_AMBIGUOUS,
    ERRMSG_TOO_MANY_ARGS,
    ERRMSG_NUMERIC_REQUI
}   t_err_msg;

typedef enum e_err_no
{
    ENO_SUCCESS,
    ENO_GENERAL,
    ENO_CANT_EXEC = 126,
    ENO_NOT_FOUND,
    ENO_EXEC_255 = 255
}   t_err_no;

typedef enum e_ast_direction
{
    TD_LEFT,
    TD_RIGHT
}   t_ast_direction;

typedef enum e_node_type
{
    N_PIPE,
    N_AND,
    N_OR,
    N_CMD,
}   t_node_type;

typedef enum e_io_type
{
    IO_IN,
    IO_OUT,
    IO_HEREDOC,
    IO_APPEND
}   t_io_type;

typedef enum e_parse_err_type
{
    E_MEM = 1,
    E_SYNTAX
}   t_parse_err_type;

typedef struct  s_io_node
{
    t_io_type           type;
    char                *value;
    char                **expanded_value;
    int                 here_doc;
    struct s_io_node    *prev;
    struct s_io_node    *next;
}   t_io_node;

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
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
    struct s_token  *prev;
}   t_token;

typedef struct s_node
{
    t_node_type     type;
    t_io_node       *io_list;
    char            *args;
    char            **expanded_args;
    struct s_node   *left;   
    struct s_node   *right;   
}   t_node;

typedef struct s_parse_err
{
    t_parse_err_type    type;
    char                *str;
}   t_parse_err;

typedef struct s_err
{
    t_err_no    no;
    t_err_msg msg;
    char    *cause;
}   t_err;

typedef struct s_path
{
    t_err   err;
    char    *path; 
}   t_path;

typedef struct s_minishell
{
    char            *line;
    t_token         *tokens;
    t_token         *curr_token;
    t_node          *ast;
    int             exit_s;
    bool            signint_child;
    t_parse_err     parse_err;
    int             stdin;
    int             stdout;
    char            **environ;
    t_env           *envlst;
    bool            heredoc_sigint;
    struct termios  original_term;
}       t_minishell;
extern t_minishell g_minishell;

t_token *ft_tokenize(void);
int ft_append_separator(t_token_type type, char **line_ptr,
    t_token **token_list);
int ft_append_identifier(char **line_ptr, t_token **token_list);
int ft_handle_separator(char **line_ptr, t_token **token_list);
t_token *ft_tokenization_handler(char *line);
t_token *ft_new_token(char *value, t_token_type type);
void    ft_token_list_add_back(t_token **lst, t_token *new_token);
void    ft_clear_token_list(t_token **lst);
int ft_is_quote(char c);
int ft_is_separator(char *s);
void ft_skip_spaces(char **line);
bool    ft_skip_quotes(char *line, size_t *i);
void    ft_print_quote_err(char c);
t_node  *ft_term(void);
t_node  *ft_expression(int min_prec);
t_node  *ft_combine (t_token_type type_op, t_node *left, t_node *right);
t_node *parsing(void);
void    ft_get_next_token(void);
bool    ft_curr_token_is_binop(void);
int ft_prec(t_token_type type);
int ft_curr_token_prec();
t_node_type ft_get_node_type(t_token_type type);
t_io_type   ft_get_io_type(t_token_type type);
t_node  *ft_new_node(t_node_type type);
t_io_node   *ft_new_io_node(t_token_type type, char *value);
void    ft_append_io_node(t_io_node **lst, t_io_node *new);
void    ft_clear_io_list(t_io_node  **lst);
void    ft_clear_cmd_node(t_node *node);
void    ft_recursive_clear_ast(t_node *node);
void    ft_clear_ast(t_node **ast);
bool    ft_get_io_list(t_io_node **io_list);
bool    ft_join_args(char **args);
t_node  *ft_get_simple_cmd(void);
void    ft_free_char2(char **tofree);
void    ft_free_char3(char ***tofree);
void    ft_set_parse_err(t_parse_err_type type);
void    ft_handle_parse_err(void);
bool    ft_is_redir(t_token_type type);
char    *ft_strjoin_with(char const *s1, char const *s2, char c);
const char* token_type_to_string(t_token_type type);
#endif