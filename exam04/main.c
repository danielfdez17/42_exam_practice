#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define FATAL_ERR "error: fatal\n"
#define RE 0
#define WE 1

int	ft_error(char *msg)
{
	while (*msg)
		write(STDERR_FILENO, msg++, 1);
	return 1;
}

int	ft_cd(char **av, int i)
{
	if (i != 2)
		return ft_error("error: cd: bad arguments\n");
	if (chdir(av[1]) == -1)
		return ft_error("error: cd: cannot change directory to "), ft_error(av[1]), ft_error("\n");
	return 0;
}

int	ft_execve(char **av, int i, char **env)
{
	int fds[2];
	int status, pid;
	int has_pipe = av[i] && !strcmp(av[i], "|");

	if (has_pipe && pipe(fds) == -1)
		return ft_error(FATAL_ERR);

	pid = fork();
	if (pid == 0)
	{
		av[i] = 0;
		if (has_pipe && (dup2(fds[1], 1) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
			ft_error("error: fatal\n"), exit(1);
		execve(*av, av, env);
		ft_error("error: cannot execute "), ft_error(*av), ft_error("\n"), exit(1);
	}
	if (waitpid(pid, &status, 0) == -1)
		return ft_error(FATAL_ERR);
	if (has_pipe && (dup2(fds[0], 0) == -1 || close(fds[0]) == -1 || close(fds[1]) == -1))
		return ft_error(FATAL_ERR);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return 1;
}

int main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;
	int i = 1, status = 0;
	int dup_stdin = dup(0);

	if (dup_stdin == -1)
		return ft_error(FATAL_ERR);
	if (ac > 1)
	{
		while (av[i])
		{
			av += i;
			i = 0;
			// ! searching cmds delimiters
			while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
				++i;
			// ? cd found, then exec cd built-in
			if (av[0] && !strcmp(av[0], "cd"))
				status = ft_cd(av, i);
			// ? otherwise, execve
			else if (i > 0)
				status = ft_execve(av, i, env);
			// ! if not another cmd and dup2 failed
			if (!av[i] || !strcmp(av[i], ";"))
				if (dup2(dup_stdin, 0) == -1)
					return ft_error(FATAL_ERR);
			if (av[i])
				++i;
		}
	}
	close(dup_stdin);
	return status;
}