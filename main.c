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

t_global_var global_var;

static void init_global_var(void)
{
	ft_memset(&global_var, 0, sizeof(t_global_var));
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
void print_branch (t_branch *node)
{
	printf("args: %s\n", node->args);
	while (node->expanded_args != NULL && *node->expanded_args != NULL && **node->expanded_args != '\0')
	{
		printf("expanded_args: %s\n", *node->expanded_args);
		(node->expanded_args)++;
	}
	printf("type: %d\n", node->type);
	if (node->io_list){
		printf("IO value: %s\n", node->io_list->value);
		if (node->io_list->expanded_value)
			printf("IO expanded_value: %s\n", *(node->io_list->expanded_value));
	}//t_io_list *tmp = node->io_list;
	// while (tmp)
	// {
	// 	printf("io expanded_value: %s\n", *(tmp->expanded_value));
	// 	printf("io value: %s\n", tmp->value);
	// 	printf("io type: %d\n", tmp->type);
	// 	tmp = tmp->next;
	// }
	
}
void print_io_list(t_io_list *io_list) {
	while (io_list) {
		switch (io_list->type) {
			case IO_IN: printf("I/O Type: IO_IN\n"); break;
			case IO_OUT: printf("I/O Type: IO_OUT\n"); break;
			case IO_HEREDOC: printf("I/O Type: IO_HEREDOC\n"); break;
			case IO_APPEND: printf("I/O Type: IO_APPEND\n"); break;
		}
		printf("Value: %s\n", io_list->value);
		if (io_list->expanded_value) {
			printf("Expanded Value: ");
			for (char **exp = io_list->expanded_value; *exp; ++exp) {
				printf("%s ", *exp);
			}
			printf("\n");
		}
		if (io_list->here_doc) {
			printf("Here Doc: %d\n", io_list->here_doc);
		}
		io_list = io_list->next;
	}
}

void print_t_branch(t_branch *node) {
	if (!node) return;

	switch (node->type) {
		case N_PIPE: printf("Node Type: N_PIPE\n"); break;
		case N_AND: printf("Node Type: N_AND\n"); break;
		case N_OR: printf("Node Type: N_OR\n"); break;
		case N_CMD: printf("Node Type: N_CMD\n"); break;
	}

	printf("Arguments: %s\n", node->args ? node->args : "NULL");

	if (node->expanded_args) {
		printf("Expanded Args: ");
		for (char **arg = node->expanded_args; *arg; ++arg) {
			printf("%s ", *arg);
		}
		printf("\n");
	}

	printf("I/O List:\n");
	print_io_list(node->io_list);

	printf("Left Child:\n");
	print_t_branch(node->left);
	
	printf("Right Child:\n");
	print_t_branch(node->right);
}

void    ft_recursive_print_ast(t_branch *node)
{
	if (!node)
		return ;
	if (node->type == N_CMD)
		print_branch(node);
	else
	{
		if (node->left)
			ft_recursive_print_ast(node->left);
		if (node->right)
			ft_recursive_print_ast(node->right);
	}
	//free(node);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	init_global_var();
	while (1)
	{
		global_var.line = readline("minishell$ ");
		if (!global_var.line)
		{
			ft_putstr_fd("exit\n", 1);
			exit(global_var.exit_s);
		}
		if (global_var.line[0])
			add_history(global_var.line);
		global_var.tokens = ft_tokenize();
		if (!global_var.tokens)
		{
			ft_putstr_fd("error with tokens! exit\n", 1);
			exit(global_var.exit_s);
		}       
		global_var.ast = parsing();
		if (global_var.parse_err)
		{
			ft_handle_parse_err();
			continue ;
		}
		//echo "gama" > x.txt && cat x.txt
		ft_recursive_print_ast(global_var.ast);
	}    
}