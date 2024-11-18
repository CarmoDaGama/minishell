/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:42:50 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/31 11:42:54 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	strim_line(int	*pos, char **b, char **input)
{	
	char	*tmp;

	*pos = ft_strlen(*input);
	tmp = mm(mjn(mm(t(*b, " "), false), mm(t(*input, " "), false), " "), false);
	if (!tmp)
		(free(*input), free(*b), exit(1));
	*b = tmp;
}

static char	*run_child(int fds[2], char **buffer)
{
	char	*input;
	int		pos;

	(signal(SIGINT, SIG_DFL), pos = 0);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			ft_putstr_fd("minishell: pipe interrupted return\n", 2);
			return (free(*buffer), exit(1), NULL);
		}
		strim_line(&pos, buffer, &input);
		if (pos > 0 && (input[pos - 1] != '|' || input[0] == '|'))
			break ;
		free(input);
	}
	return (ft_putstr_fd(*buffer, fds[1]), close(fds[1]), exit(0), *buffer);
}

static bool	is_valid_pipe(char *line)
{
	int	size;

	size = ft_strlen(line);
	return (size > 0 && (line[size - 1] == '|'
			&& line[size - 2] != '|' && *line != '|'));
}

static char	*ft_pipe_heredoc(t_global_var *global_var)
{
	char	*buffer;
	int		fds[2];
	int		status;
	pid_t	pid;

	(pipe(fds), buffer = manage_momery(ft_strdup(""), false),
		pid = fork());
	if (!pid)
		return (run_child(fds, &buffer));
	else
	{
		(signal(SIGINT, handle_in_process_son), waitpid(pid, &status, 0),
			close(fds[1]), buffer = get_next_line(fds[0]), close(fds[0]));
		if (WIFSIGNALED(status))
		{
			global_var->state_exit = (WTERMSIG(status) + 128);
			(read_command(global_var),
				exit(global_var->state_exit));
		}
		if (get_exit_status(status))
			return (NULL);
		return (buffer);
	}
}

char	*wait_pipe(char *line, t_global_var *global_var)
{
	char	*tmp;

	tmp = line;
	line = manage_momery(ft_strtrim(line, " "), false);
	if (is_valid_pipe(line))
	{
		tmp = manage_momery(ft_pipe_heredoc(global_var), false);
		if (tmp)
			line = manage_momery(ft_strjoin(line, tmp), false);
	}
	return (line);
}
