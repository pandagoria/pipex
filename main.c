/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlaunch <hlaunch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 00:10:21 by hlaunch           #+#    #+#             */
/*   Updated: 2022/01/17 01:23:10 by hlaunch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_fifo(t_child *child)
{
	child->fifo[0][0] = -1;
	child->fifo[0][1] = -1;
	child->fifo[1][0] = -1;
	child->fifo[1][1] = -1;
	child->cur_pipe = 0;
	child->i = 2;
}

void	inout_files(char *file, char flag)
{
	int	fd;

	if (flag == 'i')
	{
		fd = open(file, O_RDONLY, 0777);
		if (fd < 0)
			error_management("open");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (flag == 'o')
	{
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (fd < 0)
			error_management("open");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	exec_cmds(char **argv, const char **env, int argc, t_child *ch)
{
	if (ch->i > 2)
	{
		dup2(ch->fifo[1 - ch->cur_pipe][0], STDIN_FILENO);
		close(ch->fifo[1 - ch->cur_pipe][0]);
	}
	if (ch->i < argc - 2)
	{
		if (ch->i == 2)
			inout_files(argv[1], 'i');
		dup2(ch->fifo[ch->cur_pipe][1], STDOUT_FILENO);
		close(ch->fifo[ch->cur_pipe][0]);
		close(ch->fifo[ch->cur_pipe][1]);
	}
	if (ch->i == argc - 2)
		inout_files(argv[argc - 1], 'o');
	ch->path = get_path(argv[ch->i], env, ch);
	if (ch->cmd[0] == NULL || ch->path == NULL)
		error_management("execve");
	if (execve(ch->path, ch->cmd, (char *const *) env) == -1)
		error_management("execve");
	child_free(ch->path, ch->cmd);
}

int	main(int argc, char **argv, const char **env)
{
	t_child	child;

	if (argc < 5)
	{
		write(1, "Error: Too few arguments\n", 26);
		return (-1);
	}
	init_fifo(&child);
	while (child.i < argc - 1)
	{
		if (pipe(child.fifo[child.cur_pipe]) == -1)
			error_management("pipe");
		child.pid = fork();
		if (child.pid == -1)
			error_management("fork");
		if (child.pid == 0)
			exec_cmds(argv, env, argc, &child);
		close(child.fifo[1 - child.cur_pipe][0]);
		close(child.fifo[child.cur_pipe][1]);
		child.cur_pipe = 1 - child.cur_pipe;
		child.i++;
	}
	close(child.fifo[1 - child.cur_pipe][0]);
	wait_for_child(argc);
	return (0);
}
