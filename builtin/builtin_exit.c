/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mecavus <mecavus@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:30:00 by mecavus          #+#    #+#             */
/*   Updated: 2025/07/10 13:30:00 by mecavus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_exit_arg_validity(char *arg)
{
	int	flag;
	int	exit_code;

	flag = 0;
	exit_code = ft_atoi(arg, &flag);
	if (flag || !is_numeric(arg))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	return (exit_code % 256);
}

void	builtin_exit(char **args)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 1);
	if (!args[1])
		exit_code = exit_status(0, PULL);
	else if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		exit_status(1, PUSH);
		return ;
	}
	else
		exit_code = check_exit_arg_validity(args[1]);
	
	exit_status(exit_code, PUSH);
	
	clear_history();
	rl_clear_history();
	ft_malloc(0, CLEAR);
	exit(exit_code);
}
