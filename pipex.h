/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pndgoria <pndgoria@student.42.fr>          +#+  +:+       +#+        */
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
	char    *path;
	char    **cmd;
	pid_t   pid;
	int     *fifo;
}   t_child;

#endif