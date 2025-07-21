/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_atoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:29:16 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/21 19:11:20 by mecavus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	lenght(long nb)
{
	size_t	i;

	i = 0;
	if (nb < 0)
		nb *= -1;
	while (nb > 0)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

static char	*handle_negative(long *num, int *len, int *flag)
{
	char	*s;

	(*len)++;
	*flag = 1;
	*num *= -1;
	s = ft_malloc(*len + 1, ALLOC);
	s[0] = '-';
	return (s);
}

char	*ft_itoa(int n)
{
	long	num;
	int		len;
	int		flag;
	char	*s;

	flag = 0;
	num = n;
	len = lenght(num);
	if (!n)
		return (ft_strdup("0"));
	if (num < 0)
		s = handle_negative(&num, &len, &flag);
	else
		s = ft_malloc(len + 1, ALLOC);
	s[len] = '\0';
	len--;
	while (len >= flag)
	{
		s[len] = num % 10 + 48;
		num /= 10;
		len--;
	}
	return (s);
}

static int	check_overflow(unsigned long res, int sig, int *flag)
{
	if (res > 9223372036854775807 && sig == -1)
	{
		*flag = 1;
		return (0);
	}
	if (res > 9223372036854775807 && sig == 1)
	{
		*flag = 1;
		return (-1);
	}
	return (1);
}

int	ft_atoi(const char *str, int *flag)
{
	unsigned long	res;
	int				sig;
	int				overflow_result;

	res = 0;
	sig = 1;
	while (ft_is_space(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sig *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*(str++) - 48);
		overflow_result = check_overflow(res, sig, flag);
		if (overflow_result != 1)
			return (overflow_result);
	}
	*flag = 0;
	return ((int)res * sig);
}
