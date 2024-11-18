/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_command_in_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:57:26 by dnzita            #+#    #+#             */
/*   Updated: 2024/09/27 15:57:32 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*ft_strjoin_auxi(char const *s1, char const *s2, char *c)
{
	char	*s3;
	char	*s4;

	s3 = ft_strjoin(s1, c);
	s4 = ft_strjoin(s3, s2);
	free(s3);
	return (s4);
}

int	exec_file(char **commands, char **environ, t_global_var *global_var)
{
	int			status;
	pid_t		pid;
	struct stat	file_stat;

	if (!stat(commands[0], &file_stat) && S_ISREG(
			file_stat.st_mode) && (file_stat.st_mode & S_IXUSR))
	{
		global_var->is_child = true;
		main_signal(global_var);
		pid = fork();
		if (!pid)
			execve(commands[0], commands, environ);
		else
		{
			waitpid(pid, &status, 0);
			global_var->is_child = false;
			main_signal(global_var);
			return (WEXITSTATUS(status));
		}
	}
	return (1);
}

int	father_func(t_global_var *global_var)
{
	int	status;

	waitpid(-1, &status, 0);
	global_var->is_child = false;
	main_signal(global_var);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

int	search_command_in_path(char **commands, t_global_var *global_var)
{
	char		*str;
	pid_t		pid;
	char		**paths;
	extern char	**environ;

	if (!exec_file(commands, environ, global_var))
		return (0);
	str = ft_get_envlst_val("PATH", global_var);
	if (!str)
		return (127);
	paths = ft_split(str, ':');
	global_var->is_child = true;
	main_signal(global_var);
	pid = fork();
	while (*paths)
	{	
		if (pid)
			return (father_func(global_var));
		str = manage_momery(ft_strjoin_auxi(*paths, commands[0], "/"), false);
		execve(str, commands, environ);
		paths++;
	}
	exit(127);
}
