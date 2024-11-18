/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:04:20 by cgama             #+#    #+#             */
/*   Updated: 2024/10/09 11:04:41 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(char **args, t_global_var *global_var)
{
	if (!ft_strcmp(args[0], "echo"))
		return (ft_echo(args));
	if (!ft_strcmp(args[0], "cd"))
		return (change_directory(args, global_var));
	if (!ft_strcmp(args[0], "env"))
		return (ft_env(global_var));
	if (!ft_strcmp(args[0], "pwd"))
		return (print_working_directory());
	if (!ft_strcmp(args[0], "export"))
		return (ft_export(args, global_var));
	if (!ft_strcmp(args[0], "unset"))
		return (ft_unset(args, global_var));
	ft_exit(args, global_var);
	return (1);
}

void	ft_exit(char **args, t_global_var *global_var)
{
	int	state;

	if (args)
	{
		state = 0;
		ft_printf("exit\n");
		if (args[1] != NULL && !is_numeric(args[1]))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n",
				args[1]);
			state = 2;
			(clean_all(global_var), exit(global_var->state_exit));
		}
		else if (args[1] != NULL && args[2] != NULL)
		{
			ft_printf("minishell: exit: too many arguments\n");
		}
		else
		{
			if (args[1] != NULL)
				state = ft_atoi(args[1]);
			(clean_all(global_var), exit(state));
		}
	}
}

bool	is_builtin(char *arg)
{
	return (arg && (!ft_strcmp(arg, "echo")
			|| !ft_strcmp(arg, "cd")
			|| !ft_strcmp(arg, "exit")
			|| !ft_strcmp(arg, "pwd")
			|| !ft_strcmp(arg, "export")
			|| !ft_strcmp(arg, "unset")
			|| !ft_strcmp(arg, "env")));
}
