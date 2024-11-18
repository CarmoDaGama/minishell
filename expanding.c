/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:19:36 by cgama             #+#    #+#             */
/*   Updated: 2024/10/09 14:19:38 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_f(char *s1, char *s2)
{
	char	*joined;
	size_t	total_length;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	total_length = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = ft_calloc(total_length, sizeof(char));
	if (!joined)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = 0;
	return (free(s1), free(s2), joined);
}

char	*ft_handle_dollar(char *str, size_t *i, t_global_var *global_var)
{
	size_t	start;
	char	*key;
	char	*env_val;

	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
	{
		(*i)++;
		return (ft_strdup(""));
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(global_var->state_exit));
	}
	else if (!ft_is_valid_var_char(str[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	key = ft_substr(str, start, *i - start);
	env_val = ft_get_envlst_val(key, global_var);
	if (!env_val)
		return (free(key), ft_strdup(""));
	return (free(key), ft_strdup(env_val));
}

static char	*ft_cmd_pre_expander(char *str, t_global_var *global_var)
{
	char	*ret;
	size_t	i;

	ret = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			ret = ft_strjoin_f(ret, ft_handle_squotes(str, &i));
		else if (str[i] == '"')
			ret = ft_strjoin_f(ret, ft_handle_dquotes(str, &i, global_var));
		else if (str[i] == '$')
			ret = ft_strjoin_f(ret, ft_handle_dollar(str, &i, global_var));
		else
			ret = ft_strjoin_f(ret, ft_handle_normal_str(str, &i));
	}
	return (ret);
}

bool	is_space(char *str)
{
	return (str[0] == ' ' && str[1] == '\0');
}

char	**expanding(char *s, t_global_var *global_var)
{
	char	**expanded;
	size_t	i;

	s = ft_cmd_pre_expander(s, global_var);
	if (!s || !*s)
	{
		free(s);
		return (NULL);
	}
	s = ft_clean_empty_strs(s);
	if (!s)
		return (NULL);
	expanded = expanding_split(s);
	free(s);
	if (!expanded)
		return (NULL);
	i = 0;
	while (expanded[i])
	{
		expanded[i] = striping_quotes(expanded[i]);
		i++;
	}
	return (expanded);
}
