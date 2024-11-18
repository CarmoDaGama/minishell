/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:04:08 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/23 12:04:14 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expading_written(char *str,
	size_t i, int fd, t_global_var *gloabl_var)
{
	size_t	start;
	char	*str_aux;

	start = ++i;
	if (str[i] == '?')
		return (ft_putnbr_fd(gloabl_var->state_exit, fd), 2);
	while (str[i] && ft_is_valid_var_char(str[i]))
		i++;
	if (i != start)
	{
		str_aux = manage_momery(ft_substr(str, start, i - start), false);
		str_aux = ft_get_envlst_val(str_aux, gloabl_var);
		if (str_aux)
			ft_putstr_fd(str_aux, fd);
	}
	return (i - start + 1);
}

void	heredoc_expanding(char *str, int fd, t_global_var *global_var)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			i += expading_written(str, i, fd, global_var);
		else
			i += (ft_putchar_fd(str[i], fd), 1);
	}
	ft_putchar_fd('\n', fd);
}

void	execute_son(t_branch *branch, t_global_var *global_var)
{
	char	*str;
	char	*io_value;

	(signal(SIGINT, SIG_DFL), str = manage_momery(readline("> "), false));
	io_value = manage_momery(
			ft_strtrim(branch->io_list->value, " "), false);
	while (str && ft_strcmp(str, io_value))
	{
		(heredoc_expanding(str, branch->io_list->here_doc[1], global_var),
			str = manage_momery(readline("> "), false));
	}
	exit(0);
}

void	ft_write_heredoc(t_branch *branch, t_global_var *global_var)
{
	int		status;
	pid_t	pid;

	if (!branch->io_list || pipe(branch->io_list->here_doc) == -1)
		return ;
	pid = fork();
	if (!pid)
		execute_son(branch, global_var);
	else
	{
		(signal(SIGINT, handle_in_process_son), waitpid(pid, &status, 0),
			close(branch->io_list->here_doc[1]));
		if (WIFSIGNALED(status))
		{
			global_var->state_exit = (WTERMSIG(status) + 128);
			read_command(global_var);
			exit(global_var->state_exit);
		}
	}
}
