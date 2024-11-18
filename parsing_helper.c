/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:56:06 by cgama             #+#    #+#             */
/*   Updated: 2024/10/01 12:56:18 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_get_io_list(t_io_list **io_list, t_global_var *global_var)
{
	t_token_type	redir_type;
	t_io_list		*tmp_io_list;

	if (global_var->parse_err)
		return (false);
	while (global_var->curr_token && ft_is_redir(global_var->curr_token->type))
	{
		redir_type = global_var->curr_token->type;
		ft_get_next_token(global_var);
		if (!global_var->curr_token)
			return (ft_set_parse_err(1, global_var), false);
		if (global_var->curr_token->type != T_IDENTIFIER)
			return (ft_set_parse_err(1, global_var), false);
		tmp_io_list = ft_new_io_list(redir_type, global_var->curr_token->value);
		if (!tmp_io_list)
			return (ft_set_parse_err(1, global_var), false);
		ft_append_io_list(io_list, tmp_io_list);
		ft_get_next_token(global_var);
	}
	return (true);
}

char	*ft_strjoin_with(char const *s1, char const *s2, char c)
{
	char	*joined;
	size_t	total_length;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	if (!c || !ft_strlen(s1) || !ft_strlen(s2))
		return (ft_strjoin(s1, s2));
	total_length = ft_strlen(s1) + ft_strlen(s2) + 1 + 1;
	joined = ft_calloc(total_length, sizeof(char));
	if (!joined)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	joined[i++] = c;
	j = 0;
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = '\0';
	return (joined);
}

bool	ft_join_args(char **args, t_global_var *global_var)
{
	char	*to_free;

	if (global_var->parse_err)
		return (false);
	if (!*args)
		*args = ft_strdup("");
	if (!*args)
		return (false);
	while (global_var->curr_token
		&& global_var->curr_token->type == T_IDENTIFIER)
	{
		to_free = *args;
		*args = ft_strjoin_with(*args, global_var->curr_token->value, ' ');
		if (!*args)
			return (free(to_free), false);
		free (to_free);
		ft_get_next_token(global_var);
	}
	return (true);
}

t_branch	*ft_get_simple_cmd(t_global_var *global_var)
{
	t_branch	*node;

	if (global_var->parse_err)
		return (NULL);
	node = ft_new_branch(N_CMD);
	if (!node)
		return (ft_set_parse_err(1, global_var), NULL);
	while (global_var->curr_token
		&& (global_var->curr_token->type == T_IDENTIFIER
			|| ft_is_redir(global_var->curr_token->type)))
	{
		if (global_var->curr_token->type == T_IDENTIFIER)
		{
			if (!ft_join_args(&(node->args), global_var))
				return (ft_clear_cmd_branch(node),
					ft_set_parse_err(1, global_var), NULL);
		}
		else if (ft_is_redir(global_var->curr_token->type))
		{
			if (!ft_get_io_list(&(node->io_list), global_var))
				return (free(node->args), free(node), NULL);
		}
	}
	return (node);
}
