/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:41:52 by cgama             #+#    #+#             */
/*   Updated: 2024/10/01 14:41:58 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_set_parse_err(int type, t_global_var *global_var)
{
	global_var->parse_err = type;
}

static void	print_error(char *types[], t_token_type token_type)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(types[token_type], 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd("\n", 2);
}

void	ft_handle_parse_err(t_global_var *global_var)
{
	int		type;
	int		token_type;
	char	**types;

	types = (char *[]){"T_IDENTIFIER",
		"<", ">", "<<", ">>", "|", "(", ")", "&&", "||", "newline"};
	type = global_var->parse_err;
	(void)token_type;
	(void)types;
	if (type)
	{
		if (type > 0)
		{
			if (!global_var->curr_token)
				token_type = T_NL;
			else
				token_type = global_var->curr_token->type;
			print_error(types, (t_token_type)token_type);
			global_var->state_exit = 258;
		}
		global_var->parse_err = 0;
		ft_clear_ast(&global_var->ast, global_var);
	}
}
