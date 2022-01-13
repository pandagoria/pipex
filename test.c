/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlaunch <marvin@student.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 01:28:55 by hlaunch           #+#    #+#             */
/*   Updated: 2022/01/13 17:49:49 by hlaunch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*int	parse_cmd(char *cmd, const char **env)
{
	int		i;
	char	*path;

	i = 0;
	while (*env[i])
	{
		if (ft_strncmp("PATH=", env[i], 6) == 0)
			return (env[i]);
		
	}
}*/
int main(int argc, char **argv, const char **env)
{
	pid_t	cmd1;
	pid_t	cmd2;
	int		fifo[2];
	int		fd;
	char	*arg[4];

	if (argc != 5)
		return (-1);
	arg[0] = "sh";
	arg[1] = "-c";
	arg[3] = 0;
	if (pipe(fifo) == -1)
	{
		perror("pipe");
		exit(1);
	}
	cmd1 = fork();
	if (!cmd1)
	{
		arg[2] = argv[2];
		fd = open(argv[1], O_RDONLY);
		dup2(fd, STDIN_FILENO);
		dup2(fifo[1], STDOUT_FILENO);
		close(fifo[0]);
		close(fifo[1]);
		execve("/bin/sh", arg, (char *const *) env);
	}
	cmd2 = fork();
	if (!cmd2)
	{
		arg[2] = argv[3];
		fd = open(argv[4], O_CREAT | O_WRONLY, S_IRUSR
				| S_IWUSR | S_IRGRP | S_IROTH);
		dup2(fifo[0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fifo[0]);
		close(fifo[1]);
		execve("/bin/sh", arg, (char *const *) env);
	}
	close(fifo[0]);
	close(fifo[1]);
	while (wait(&cmd1) != cmd1 || wait(&cmd2) != cmd2)
		return (1);
	return (0);
}
