/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_all.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:31:31 by cgama             #+#    #+#             */
/*   Updated: 2024/10/14 15:31:39 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_envlst(t_global_var *global_var)
{
	t_env	*envlist;
	t_env	*envlist_tmp;

	envlist = global_var->envlst;
	while (envlist)
	{
		envlist_tmp = envlist;
		envlist = envlist->next;
		free(envlist_tmp);
	}
	global_var->envlst = NULL;
}

void	clean_all(t_global_var *global_var)
{
	manage_momery(NULL, true);
	ft_clear_ast(&global_var->ast, global_var);
	free_envlst(global_var);
	rl_clear_history();
	if (global_var->line)
		free(global_var->line);
}

char	*t(char const *s1, char const *set)
{
	return (ft_strtrim(s1, set));
}
