/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlaunch <hlaunch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 23:50:50 by hlaunch           #+#    #+#             */
/*   Updated: 2022/01/17 01:23:40 by hlaunch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	wait_for_child(int argc)
{
	int	i;

	i = 0;
	while (i < argc - 1)
	{
		waitpid(-1, 0, 0);
		i++;
	}
}

void	child_free(char *str, char **mat)
{
	int	i;

	i = 0;
	if (str != NULL)
		free(str);
	if (mat != NULL)
	{
		while (mat[i])
			free(mat[i++]);
		free(mat);
	}
}

void	error_management(const char *error_msg)
{
	perror(error_msg);
	exit(2);
}
