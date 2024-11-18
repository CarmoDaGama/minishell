/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:06:26 by cgama             #+#    #+#             */
/*   Updated: 2024/09/30 11:06:28 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_tokenize(t_global_var *global_var)
{
	t_token	*token_list;

	token_list = ft_tokenization_handler(global_var);
	global_var->line = NULL;
	return (token_list);
}

int	ft_append_separator(t_token_type type, char **line_ptr,
	t_token **token_list)
{
	t_token	*token;

	token = ft_new_token(NULL, type);
	if (!token)
		return (0);
	ft_token_list_add_back(token_list, token);
	(*line_ptr)++;
	if (type == T_DLESS || type == T_DGREAT || type == T_OR || type == T_AND)
		(*line_ptr)++;
	return (1);
}

int	ft_append_identifier(
		char **line_ptr, t_token **token_list, t_global_var *global_var)
{
	char	*tmp_line;
	char	*value;
	t_token	*token;
	size_t	i;

	tmp_line = *line_ptr;
	i = 0;
	while (tmp_line[i] && !ft_is_separator(tmp_line + i))
	{
		if (ft_is_quote(tmp_line[i]))
		{
			if (!ft_skip_quotes(tmp_line, &i))
				return (ft_print_quote_err(tmp_line[i], global_var), 0);
		}
		else
			i++;
	}
	value = manage_momery(ft_substr(tmp_line, 0, i), false);
	if (!value)
		return (0);
	token = ft_new_token(value, T_IDENTIFIER);
	if (!token)
		return (0);
	*line_ptr += i;
	return (ft_token_list_add_back(token_list, token), 1);
}

int	ft_handle_separator(char **line_ptr, t_token **token_list)
{
	if (!ft_strncmp(*line_ptr, "<<", 2))
		return (ft_append_separator(T_DLESS, line_ptr, token_list) && 1);
	else if (!ft_strncmp(*line_ptr, ">>", 2))
		return (ft_append_separator(T_DGREAT, line_ptr, token_list) && 1);
	else if (!ft_strncmp(*line_ptr, "<", 1))
		return (ft_append_separator(T_LESS, line_ptr, token_list) && 1);
	else if (!ft_strncmp(*line_ptr, ">", 1))
		return (ft_append_separator(T_GREAT, line_ptr, token_list) && 1);
	else
		return (ft_append_separator(T_PIPE, line_ptr, token_list) && 1);
}

t_token	*ft_tokenization_handler(t_global_var *global_var)
{
	char	*line;
	int		error;
	t_token	*token_list;

	error = 0;
	token_list = NULL;
	line = global_var->line;
	while (*line)
	{
		if (error)
			return (NULL);
		if (ft_isspace(*line))
			ft_skip_spaces(&line);
		else if (ft_is_separator(line))
		{
			error = (!ft_handle_separator(&line, &token_list) && 1);
		}
		else
			error = (!ft_append_identifier(
						&line, &token_list, global_var) && 1);
	}
	return (token_list);
}
