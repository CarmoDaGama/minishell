/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_working_directory.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 08:32:33 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/14 08:32:36 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_working_directory(void)
{
	char	*buf;

	buf = NULL;
	buf = getcwd(buf, 0);
	printf("%s\n", buf);
	free(buf);
	return (0);
}
