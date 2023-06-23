/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotavare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:57:28 by alexandre         #+#    #+#             */
/*   Updated: 2023/06/23 22:13:21 by jotavare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	prints the string after echo command to the terminal
	with a new line at the end of the string
*/

int	echo(t_attr att)
{
	int	i;
	int	fl_n;
	int	fl_pr;

	i = 1;
	fl_n = 0;
	fl_pr = 0;
	while (i < att.nb_tokens && att.tok_arr[i])
	{
		if (handle_echo_options(att, &fl_n, &fl_pr, i))
		{
			i++;
			continue ;
		}
		if (fl_pr)
			printf(" ");
		printf("%s", att.tok_arr[i]);
		fl_pr = 1;
		i++;
	}
	if (fl_n == 0)
		printf("\n");
	return (0);
}

/*
	checks if the string after echo command is an option
	-n = no new line at the end of the string
	$? = prints the exit status of the last command
*/

int	handle_echo_options(t_attr att, int *fl_n, int *fl_pr, int i)
{
	if (ft_strnstr(att.tok_arr[i], "-n", ft_strlen(att.tok_arr[i]))
		&& *fl_pr == 0)
	{
		echo_n(att);
		*fl_n = 1;
		return (1);
	}
	if (ft_strnstr(att.tok_arr[i], "$?", ft_strlen(att.tok_arr[i])))
	{
		printf("%d", g_value);
		return (1);
	}
	return (0);
}

/*
	prints the string after echo command to the terminal
	with the -n option = no new line at the end of the string
*/

int	echo_n(t_attr att)
{
	int	i;

	i = 1;
	if (ft_strcmp(att.tok_arr[i], "-n") != 0)
		ft_putstr_fd(att.tok_arr[i], 1);
	i++;
	return (0);
}
