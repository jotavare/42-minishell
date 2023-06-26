/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotavare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:14:25 by lde-sous          #+#    #+#             */
/*   Updated: 2023/06/23 22:02:01:26 by jotavare         ###   ########.fr    */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_core(t_attr *att, t_exec *args)
{
	if (args->command[0] == '/')
		exec_absolute_path(args, att);
	else if (args->command[0] == '.')
		exec_binaries(args, att);
	else
		exec_commands(args, att);
	printf("%s: command not found \n", att->tok_arr[0]);
	exit(127);
}

void	check_flags(t_attr *att, int index)
{
	if (att->only_create)
		file_create_only(att);
	if (att->skip)
		exit(g_value);
	if (att->read_from_pipe)
		read_from_pipe(att);
	else if (att->read_from_file)
	{
		if (read_from_file(att, index) < 0)
			exit(g_value);
	}
	if (att->heredoc)
		heredoc(att->commands_arr[att->i + 2], att);
	if (att->write_to_pipe && att->read_from_pipe)
		att->pipeindex++;
	if (att->write_to_pipe)
		write_to_pipe(att);
	if (att->redir)
		redir_append(att, index);
}

void	executer(t_attr *att, t_exec *args)
{
	if (!ft_strcmp(att->tok_arr[0], "pwd"))
		g_value = pwd();
	else if (!ft_strcmp(att->tok_arr[0], "echo"))
		g_value = echo(*att);
	else if (!ft_strcmp(att->tok_arr[0], "env"))
		g_value = env(att);
	else if (ft_strcmp(att->tok_arr[0], "export") == 0)
		export_print(*att);
	else
		execute_core(att, args);
}

int	execute(t_attr *att, int index)
{
	t_exec	args;
	//int		exit_status;

	start_args(&args, att);
	args.pid = fork();
	if (args.pid == -1)
		return (-1);
	if (args.pid == 0)
	{
		check_flags(att, index);
		executer(att, &args);
		free_child(att, &args);
		//exit(g_value);
	}
	else
		waitpid(args.pid, &g_value, 0);
	if (att->read_from_pipe)
		att->pipeindex++;
	close_pipeline(att);
	free_start_args(&args);
	g_value = WEXITSTATUS(g_value);
	return (g_value);
}
