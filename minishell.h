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

typedef enum e_branch_type
{
    N_PIPE,
    N_AND,
    N_OR,
    N_CMD,
}   t_branch_type;

typedef enum e_io_type
{
    IO_IN,
    IO_OUT,
    IO_HEREDOC,
    IO_APPEND
}   t_io_type;

typedef struct  s_io_list
{
    t_io_type           type;
    char                *value;
    char                **expanded_value;
    int                 here_doc;
    struct s_io_list    *prev;
    struct s_io_list    *next;
}   t_io_list;

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

typedef struct s_branch
{
    t_branch_type     type;
    t_io_list       *io_list;
    char            *args;
    char            **expanded_args;
    struct s_branch   *left;   
    struct s_branch   *right;   
}   t_branch;



typedef struct s_global_var
{
    char            *line;
    t_token         *tokens;
    t_token         *curr_token;
    t_branch          *ast;
    int             exit_s;
    int     parse_err;
}       t_global_var;
extern t_global_var global_var;

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
t_branch  *get_term(void);
t_branch  *get_sequence(int min_prec);
t_branch  *join_branch (t_token_type type_op, t_branch *left, t_branch *right);
t_branch *parsing(void);
void    ft_get_next_token(void);
bool    ft_curr_token_is_binop(void);
int ft_prec(t_token_type type);
int ft_curr_token_prec();
t_branch_type ft_get_branch_type(t_token_type type);
t_io_type   ft_get_io_type(t_token_type type);
t_branch  *ft_new_branch(t_branch_type type);
t_io_list   *ft_new_io_list(t_token_type type, char *value);
void    ft_append_io_list(t_io_list **lst, t_io_list *new);
void    ft_clear_io_list(t_io_list  **lst);
void    ft_clear_cmd_branch(t_branch *node);
void    ft_recursive_clear_ast(t_branch *node);
void    ft_clear_ast(t_branch **ast);
bool    ft_get_io_list(t_io_list **io_list);
bool    ft_join_args(char **args);
t_branch  *ft_get_simple_cmd(void);
void    ft_free_char2(char **tofree);
void    ft_free_char3(char ***tofree);
void    ft_set_parse_err(int type);
void    ft_handle_parse_err(void);
bool    ft_is_redir(t_token_type type);
char    *ft_strjoin_with(char const *s1, char const *s2, char c);
const char* token_type_to_string(t_token_type type);
#endif