/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:05:46 by cgama             #+#    #+#             */
/*   Updated: 2024/10/18 16:05:51 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_export_err_msg(char *id)
{
	ft_putstr_fd("minishell: export : `", 2);
	ft_putstr_fd(id, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static void	ft_export_list(t_global_var *global_var)
{
	t_env	*envs;
	size_t	i;

	envs = global_var->envlst;
	while (envs)
	{
		if (envs->value != NULL && (ft_strcmp(envs->key, "_") != '\0'))
		{
			ft_printf("declare -x %s=\"", envs->key);
			i = 0;
			while (envs->value[i])
			{
				if ((envs->value)[i] == '$' || (envs->value)[i] == '"')
					ft_printf("\\%c", (envs->value[i++]));
				else
					ft_printf("%c", (envs->value[i++]));
			}
			ft_printf("\"\n");
		}
		else if (envs->value == NULL && (ft_strcmp(envs->key, "_") != 0))
			ft_printf("declare -x %s\n", envs->key);
		envs = envs->next;
	}
}

int	ft_check_key(char *str)
{
	int	i;

	i = 1;
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **argv, t_global_var *global_var)
{
	int		i;
	int		state_exit;
	char	*key;

	state_exit = 0;
	i = 1;
	if (!argv[i])
		return (ft_export_list(global_var), 0);
	while (argv[i])
	{
		if (ft_check_key(argv[i]) == 0)
			state_exit = ft_export_err_msg(argv[i]);
		else
		{
			key = ft_extract_key(argv[i]);
			if (ft_env_entry_exists(key, global_var))
				ft_update_envlst(key,
					ft_extract_value(argv[i]), false, global_var);
			else
				ft_update_envlst(key,
					ft_extract_value(argv[i]), true, global_var);
		}
		i++;
	}
	return (state_exit);
}
