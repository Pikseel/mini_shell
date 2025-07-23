/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emrozmen <emrozmen@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:30:00 by mecavus           #+#    #+#             */
/*   Updated: 2025/07/23 12:53:34 by emrozmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
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
	int		flag;
	int		exit_code;
	char	*trimmed_arg;

	trimmed_arg = ft_strtrim(arg, ' ');
	if (!trimmed_arg)
		return (2);
	flag = 0;
	exit_code = ft_atoi(trimmed_arg, &flag);
	if (flag || !is_numeric(trimmed_arg))
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
	int		exit_code;
	t_main	*shell;

	ft_putstr_fd("exit\n", 1);
	if (!args[1])
		exit_code = exit_status(0, PULL);
	else
	{
		exit_code = check_exit_arg_validity(args[1]);
		if (exit_status(2, PUSH), exit_code == 2)
		{
			shell = (t_main *)ft_malloc(0, GET_SHELL);
			clear_exit(shell, 2, NULL);
			return ;
		}
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			exit_status(1, PUSH);
			return ;
		}
	}
	exit_status(exit_code, PUSH);
	shell = (t_main *)ft_malloc(0, GET_SHELL);
	clear_exit(shell, exit_code, NULL);
}
