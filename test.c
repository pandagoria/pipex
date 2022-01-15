#include "pipex.h"

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

char    *check_cmd_args(char *arg, t_child *child)
{
	int     i;
	char *tmp;
	char *cmd;

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
	char 	**all_paths;
	char 	*cmd;
	char 	*res;

	res = NULL;
	env_path = NULL;
	all_paths = NULL;
	if (ft_strchr(argv, '/'))
		return (argv);
	cmd = check_cmd_args(argv, child);
	env_path = find_path_env(env);
	all_paths = ft_split(env_path, ':');
	res = find_cmd_path(all_paths, cmd);
	if (all_paths)
	{
		for (int j = 0; j < 9; j++)
			free(all_paths[j]);
		free(all_paths);
	}
	if (env_path)
		free(env_path);
	if (cmd)
		free(cmd);
	return (res);
}

int	first_command(char **argv, const char **env, t_child *child)
{
	int fd;

	child->path = get_path(argv[2], env, child);
	fd = open(argv[1], O_RDONLY, 0777);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("STDINFILE");
	close(fd);
	if (dup2(child->fifo[1], STDOUT_FILENO) == -1)
		perror("STDOUTFILE");
	close(child->fifo[0]);
	close(child->fifo[1]);
	execve(child->path, child->cmd, (char *const *) env);
	if (child->path)
		free(child->path);
	if (child->cmd)
	{
		for (int j = 0; j < 3; j++)
			free(child->cmd[j]);
		free(child->cmd);
	}
	return (0);
}

pid_t second_command(char **argv, const char **env, t_child *child)
{
	int fd;

	child->path = get_path(argv[3], env, child);
	fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (dup2(child->fifo[0], STDIN_FILENO) == -1)
		perror("STDINFILE");
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("STDOUTFILE");
	close(fd);
	close(child->fifo[0]);
	close(child->fifo[1]);
	execve(child->path, child->cmd, (char *const *) env);
	if (child->path)
		free(child->path);
	if (child->cmd)
	{
		for (int j = 0; j < 1; j++)
			free(child->cmd[j]);
		free(child->cmd);
	}
	return (0);
}

int main(int argc, char **argv, const char **env)
{
	t_child	child1;
	t_child	child2;
	int		fifo[2];

	if (argc != 5)
		return (-1);
	if (pipe(fifo) == -1)
	{
		perror("pipe");
		return (-1);
	}
	child1.pid = fork();
	if (child1.pid < 0)
	{
		perror("fork first");
		return (-1);
	}
	child1.fifo = fifo;
	if (!child1.pid)
	{
		if (first_command(argv, env, &child1) == -1)
			return (-1);
	}
	child2.pid = fork();
	if (child2.pid < 0)
	{
		perror("fork second");
		return (-1);
	}
	child2.fifo = fifo;
	if (!child2.pid)
	{
		if (second_command(argv, env, &child2) == -1)
			return (-1);
	}
	if (waitpid(0, &child1.pid, 0) < 0 && waitpid(0, &child2.pid, 0) < 0)
	{
		perror("waitpid");
		return (-1);
	}
	return (0);
}