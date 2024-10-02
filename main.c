/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:26:14 by cgama             #+#    #+#             */
/*   Updated: 2024/09/30 13:26:16 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

t_minishell g_minishell;

static void init_global_var(char **env)
{
    ft_memset(&g_minishell, 0, sizeof(t_minishell));
    g_minishell.environ = env;
    //->
    g_minishell.stdin = dup(0);
    g_minishell.stdout = dup(1);
    tcgetattr(STDIN_FILENO, &g_minishell.original_term);
}

const char* token_type_to_string(t_token_type type)
{
    switch (type)
    {
        case T_IDENTIFIER: return "T_IDENTIFIER";
        case T_LESS: return "T_LESS";
        case T_GREAT: return "T_GREAT";
        case T_DLESS: return "T_DLESS";
        case T_DGREAT: return "T_DGREAT";
        case T_PIPE: return "T_PIPE";
        case T_O_PARENT: return "T_O_PARENT";
        case T_C_PARENT: return "T_C_PARENT";
        case T_AND: return "T_AND";
        case T_OR: return "T_OR";
        case T_NL: return "T_NL";
        default: return "UNKNOWN_TOKEN_TYPE";
    }
}
void print_node (t_node *node)
{
    ft_printf("args: %s\n", node->args);
    while (node->expanded_args != NULL && *node->expanded_args != NULL && **node->expanded_args != '\0')
    {
        ft_printf("expanded_args: %s\n", *node->expanded_args);
        (node->expanded_args)++;
    }
    ft_printf("type: %d\n", node->type);
    t_io_node *tmp = node->io_list;
    while (tmp)
    {
        ft_printf("io expanded_value: %s\n", node->io_list->expanded_value);
        ft_printf("io value: %s\n", node->io_list->value);
        ft_printf("io type: %d\n", node->io_list->type);
        tmp = tmp->next;
    }
    
}

void    ft_recursive_print_ast(t_node *node)
{
    if (!node)
        return ;
    if (node->type == N_CMD)
        print_node(node);
    else
    {
        if (node->left)
            ft_recursive_print_ast(node->left);
        if (node->right)
            ft_recursive_print_ast(node->right);
    }
    //free(node);
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    init_global_var(env);
    while (1)
    {
        g_minishell.line = readline("minishell$ ");
        if (!g_minishell.line)
        {
            ft_putstr_fd("exit\n", 1);
            exit(g_minishell.exit_s);
        }
        if (g_minishell.line[0])
            add_history(g_minishell.line);
        g_minishell.tokens = ft_tokenize();
        if (!g_minishell.tokens)
        {
            ft_putstr_fd("error with tokens! exit\n", 1);
            exit(g_minishell.exit_s);
        }       
        g_minishell.ast = parsing();
        if (g_minishell.parse_err.type)
		{
			ft_handle_parse_err();
			continue ;
		}
        //echo "gama" > x.txt && cat x.txt
        ft_recursive_print_ast(g_minishell.ast);
    }    
}