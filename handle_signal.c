/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 08:26:00 by dnzita            #+#    #+#             */
/*   Updated: 2024/09/27 08:26:03 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	handle_newline(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_in_process_son(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
}

void	main_signal(t_global_var *global_var)
{
	if (global_var->is_child)
		signal(SIGINT, handle_in_process_son);
	else
		signal(SIGINT, handle_newline);
	signal(SIGQUIT, SIG_IGN);
}
