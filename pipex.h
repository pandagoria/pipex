/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlaunch <hlaunch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 17:24:22 by pndgoria          #+#    #+#             */
/*   Updated: 2022/01/13 17:24:22 by pndgoria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/errno.h>
# include "libft/libft.h"

typedef struct s_child {
	char	*path;
	char	**cmd;
	pid_t	pid;
	int		fifo[2][2];
	int		cur_pipe;
	int		i;
}	t_child;

char	*get_path(char *argv, const char **env, t_child *child);
void	child_free(char *str, char **mat);
void	wait_for_child(int argc);
void	error_management(const char *error_msg);

#endif