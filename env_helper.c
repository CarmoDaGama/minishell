/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:35:54 by cgama             #+#    #+#             */
/*   Updated: 2024/10/09 15:35:56 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_env_entry_exists(char *key, t_global_var *global_var)
{
	t_env	*envlst;

	envlst = global_var->envlst;
	while (envlst)
	{
		if (!ft_strcmp(key, envlst->key))
			return (true);
		envlst = envlst->next;
	}
	return (false);
}

static	t_env	*ft_envlst_new(char *key, char *value)
{
	t_env	*new;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = manage_momery(ft_strdup(key), false);
	if (value)
		new->value = manage_momery(ft_strdup(value), false);
	new->next = NULL;
	return (new);
}

char	*ft_get_envlst_val(char *key, t_global_var *global_var)
{
	t_env	*envlst;

	envlst = global_var->envlst;
	while (envlst)
	{
		if (!ft_strcmp(key, envlst->key))
			return (envlst->value);
		envlst = envlst->next;
	}
	return (NULL);
}

void	ft_envlst_back(t_env *new, t_global_var *global_var)
{
	t_env	*curr;

	if (!global_var->envlst || ft_strcmp(new->key, global_var->envlst->key) < 0)
	{
		new->next = global_var->envlst;
		global_var->envlst = new;
		return ;
	}
	curr = global_var->envlst;
	while (curr && curr->next && ft_strcmp(new->key, curr->next->key) > 0)
		curr = curr->next;
	new->next = curr->next;
	curr->next = new;
}

void	ft_update_envlst(char *key, char *value,
	bool create, t_global_var *global_var)
{
	t_env	*envlst;

	envlst = global_var->envlst;
	while (envlst)
	{
		if (!ft_strcmp(key, envlst->key))
		{
			if (value)
				envlst->value = manage_momery(ft_strdup(value),
						false);
			return ;
		}
		envlst = envlst->next;
	}
	if (create)
		ft_envlst_back(ft_envlst_new(key, value), global_var);
}
