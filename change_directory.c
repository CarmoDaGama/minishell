/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:53:43 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/31 16:43:23 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_directory(char **cd, t_global_var *global_var)
{
	int	status;

	if (!cd[1])
	{
		status = chdir(ft_get_envlst_val("HOME", global_var));
		ft_update_envlst("OLDPWD", ft_get_envlst_val("PWD", global_var),
			false, global_var);
		ft_update_envlst("PWD", manage_momery(getcwd(NULL, 0), false),
			false, global_var);
		return (status);
	}
	else if (cd[2])
		return (ft_putstr_fd("bash: cd: too many arguments\n", 2), 1);
	if (chdir(cd[1]) == -1)
	{
		(ft_putstr_fd("bash: cd: ", 2), ft_putstr_fd(cd[1], 2),
			ft_putstr_fd(" No such file or directory\n", 2));
		return (1);
	}
	ft_update_envlst("OLDPWD", ft_get_envlst_val("PWD", global_var),
		false, global_var);
	ft_update_envlst("PWD", manage_momery(getcwd(NULL, 0), false),
		false, global_var);
	return (0);
}
