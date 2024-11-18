/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:04:03 by cgama             #+#    #+#             */
/*   Updated: 2024/09/30 13:04:04 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int	ft_is_separator(char *s)
{
	if (*s == ' ' || *s == '\t'
		|| *s == '<' || *s == '>' || *s == '|')
		return (1);
	return (0);
}

void	ft_skip_spaces(char **line)
{
	while (**line && ft_isspace(**line))
		(*line)++;
}

bool	ft_skip_quotes(char *line, size_t *i)
{
	char	quote;

	quote = line[*i];
	if (ft_strchr(line + *i + 1, quote))
	{
		(*i)++;
		while (line[*i] != quote)
			(*i)++;
		(*i)++;
		return (true);
	}
	return (false);
}

void	ft_print_quote_err(char c, t_global_var *global_var)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching ", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	global_var->state_exit = 258;
}
