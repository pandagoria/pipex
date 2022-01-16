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

char	*find_path_env(const char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			return (ft_substr(env[i], 5, ft_strlen(env[i])));
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], cmd);
		if (access(tmp, 0) == 0)
			return (tmp);
		if (tmp)
			free(tmp);
		i++;
	}
	return (NULL);
}

char	*check_cmd_args(char *arg, t_child *child)
{
	int		i;
	char	*tmp;
	char	*cmd;

	if (ft_strchr(arg, ' '))
	{
		i = 0;
		while (*(arg + i) != ' ')
			i++;
		tmp = ft_substr(arg, 0, i);
		cmd = ft_strjoin("/", tmp);
		child->cmd = ft_split(arg, ' ');
		free(tmp);
	}
	else
	{
		tmp = arg;
		cmd = ft_strjoin("/", tmp);
		child->cmd = ft_split(arg, '\0');
	}
	return (cmd);
}

char	*get_path(char *argv, const char **env, t_child *child)
{
	char	*env_path;
	char	**all_paths;
	char	*cmd;
	char	*res;

	res = NULL;
	env_path = NULL;
	all_paths = NULL;
	if (ft_strchr(argv, '/'))
		return (argv);
	cmd = check_cmd_args(argv, child);
	env_path = find_path_env(env);
	all_paths = ft_split(env_path, ':');
	res = find_cmd_path(all_paths, cmd);
	child_free(cmd, all_paths);
	if (env_path)
		free(env_path);
	return (res);
}
