/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 06:10:59 by cgama             #+#    #+#             */
/*   Updated: 2024/07/28 06:11:03 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/libft.h"

# if defined (__linux__)
#  define PTRNULL "(nil)"
# elif defined (__APPLE__)
#  define PTRNULL "0x0"
# endif

typedef struct s_flags
{
	int	spec;
	int	width;
	int	left;
	int	zero;
	int	star;
	int	precision;
	int	hash;
	int	space;
	int	plus;
}		t_flags;

t_flags	initflags(void);
t_flags	setleft(t_flags flags);
t_flags	setdigit(char c, t_flags flags);
t_flags	setwidth(va_list args, t_flags flags);
int		setprecision(const char *str, int pos,
			va_list args, t_flags *flags);

int		ft_printf(const char *format, ...);
int		printer(char type, va_list args, t_flags flags);

int		printchar(char c, t_flags flags);
int		ft_putchar(char c);

int		put_str(const char *str, t_flags flags);
int		putstr(const char *str);
int		put_pre(const char *str, int precision);
int		putsign(int n, t_flags *flags);

int		putint(int n, t_flags flags);
int		putinteger(char *nbstr, int n, t_flags flags);
int		put_i(char *nbstr, int n, t_flags flags);

int		putunsigned(unsigned int n, t_flags flags);
int		put_u(char *nbstr, t_flags flags);
int		putunint(char *nbstr, t_flags flags);

int		ft_puthex(unsigned int n, int is_upper, t_flags flags);
int		putx(char *nbstr, int n, int is_upper, t_flags flags);
int		puthex(char *nbstr, int n, int is_upper, t_flags flags);

int		putpointer(unsigned long int n, t_flags flags);
int		putpointer_pre(unsigned long int n);
void	putnbr(unsigned long int n);

char	*itoa(long nb);
char	*utoa(unsigned int nb);
char	*xtoa(unsigned long int nb, int is_upper);
int		ft_unintlen(unsigned int n);
int		ft_hexlen(unsigned int n);
int		pointerlen(unsigned long int n);
int		ft_istype(int c);
int		ft_isspec(int c);
int		ft_isflag(int c);

int		putspace(int total_width, int size, int zero);

#endif
