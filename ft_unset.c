/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:10:19 by cgama             #+#    #+#             */
/*   Updated: 2024/10/22 11:10:23 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_util(char *key, t_global_var *global_var)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = global_var->envlst;
	while (current)
	{
		if (!ft_strcmp(key, current->key))
		{
			if (prev)
				prev->next = current->next;
			else
				global_var->envlst = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

bool	ft_unset(char **args, t_global_var *global_var)
{
	int		i;
	bool	err;

	i = 1;
	if (!args[0])
		return (0);
	err = false;
	while (args[i])
	{
		if (!ft_check_key(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			err = true;
		}
		else
			unset_util(ft_extract_key(args[i]), global_var);
		i++;
	}
	return (err);
}
