/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:26:14 by cgama             #+#    #+#             */
/*   Updated: 2024/09/30 13:26:16 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
ft_wait_pipe.c: Error!
ft_write_heredoc.c: Error!
parsing_error.c: Error! cgama
parsing_helper.c: Error!
striping_quotes.c: Error!
tokenizer.c: Error!
tokens_manager.c: Error!
*/
#include "minishell.h"

static t_global_var	init_global_var(char **env)
{
	t_global_var	global_var;

	ft_memset(&global_var, 0, sizeof(t_global_var));
	global_var.env = env;
	global_var.state_exit = 0;
	ft_init_envlst(&global_var);
	return (global_var);
}

bool	tokenize_parsing(t_global_var *global_var)
{
	global_var->tokens = ft_tokenize(global_var);
	if (!global_var->tokens)
		return (false);
	global_var->ast = parsing(global_var);
	if (global_var->parse_err)
	{
		ft_handle_parse_err(global_var);
		return (false);
	}
	return (true);
}

void	read_command(t_global_var *global_var)
{
	while (1)
	{
		main_signal(global_var);
		global_var->line = manage_momery(readline("minishell$ "), false);
		if (!global_var->line)
		{
			clean_all(global_var);
			ft_putstr_fd("exit\n", 1);
			exit(global_var->state_exit);
		}
		if (global_var->line[0])
		{
			global_var->line = wait_pipe(global_var->line, global_var);
			add_history(global_var->line);
		}
		if (!tokenize_parsing(global_var))
			continue ;
		init_end_branch(global_var->ast, global_var);
		call_command(global_var->ast, global_var);
		ft_clear_ast(&global_var->ast, global_var);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_global_var	global_var;

	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("bash : too many arguments", 2);
		return (1);
	}
	global_var = init_global_var(env);
	read_command(&global_var);
	manage_momery(NULL, true);
	return (clean_all(&global_var), global_var.state_exit);
}
