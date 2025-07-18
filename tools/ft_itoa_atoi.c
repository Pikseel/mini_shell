/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_atoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:29:16 by emrozmen          #+#    #+#             */
/*   Updated: 2025/07/18 15:40:15 by mecavus          ###   ########.fr       */
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
	else if (num < 0)
	{
		len++;
		flag = 1;
		num *= -1;
	}
	s = ft_malloc (len + 1, ALLOC);
	if (flag == 1)
		s[0] = '-';
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

int	ft_atoi(const char *str, int *flag)
{
	unsigned long	res;
	int				sig;

	res = 0;
	sig = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
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
	}
	*flag = 0;
	return ((int)res * sig);
}
