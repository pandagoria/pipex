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
		free(tmp);
		i++;
	}
	return (NULL);
}

char	*parse_cmdpath(char *argv, const char **env)
{
	char	*env_path;
	char 	**paths;
	char 	*cmd;
	char 	*res;

	if (ft_strchr(argv, '/'))
		return (argv);
	cmd = ft_strjoin("/", argv);
	env_path = find_path_env(env);
	paths = ft_split(env_path, ':');
	res = find_cmd_path(paths, cmd);
	for (int j = 0; j < 9; j++)
		free(paths[j]);
	free(env_path);
	free(cmd);
	free(paths);
	return (res);
}

int	first_command(char **argv, const char **env, int *fifo)
{
	int fd;
	char	*cmd_path;
	
	cmd_path = parse_cmdpath(argv[2], env);
	fd = open(argv[1], O_RDONLY, 0777);
	dup2(fd, STDIN_FILENO);
	close(fd);
	dup2(fifo[1], STDOUT_FILENO);
	close(fifo[0]);
	close(fifo[1]);
	printf("%s %s\n", cmd_path, argv[2]);
	execve(cmd_path, ft_split(argv[2], ' '), (char *const *) env);
	free(cmd_path);
	return (0);
}

pid_t second_command(char **argv, const char **env, int *fifo)
{
	int fd;
	char	*cmd_path;

	cmd_path = parse_cmdpath(argv[3], env);
	fd = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	dup2(fifo[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	close(fifo[0]);
	close(fifo[1]);
	execve(cmd_path, ft_split(argv[3], ' '), (char *const *) env);
	free(cmd_path);
	return (0);
}

int main(int argc, char **argv, const char **env)
{
	pid_t	cmd1;
	pid_t	cmd2;
	int		fifo[2];

	if (argc != 5)
		return (-1);
	if (pipe(fifo) == -1)
	{
		perror("pipe");
		exit(1);
	}
	cmd1= fork();
	if (!cmd1)
		first_command(argv, env,  fifo);
	cmd2 = fork();
	if (!cmd2)
		second_command(argv, env, fifo);
	waitpid(0, NULL, 0);
	return (0);
}