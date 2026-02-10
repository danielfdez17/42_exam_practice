#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void close_fds(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	int fds[2];
	pid_t pid;
	if (!file || !argv || (type != 'r' && type != 'w'))
		return -1;
	if (pipe(fds) < 0)
		return -1;
	pid = fork();
	if (pid < 0)
	{
		close_fds(fds);
		return -1;
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if (dup2(fds[1], STDOUT_FILENO) < 0)
			{
				close_fds(fds);
				exit(1);
			}
		}
		else
		{
			if (dup2(fds[0], STDIN_FILENO) < 0)
			{
				close_fds(fds);
				exit(1);
			}
		}
		close_fds(fds);
		execvp(file, argv);
		exit(1);
	}
	else
	{
		if (type == 'r')
		{
			close(fds[1]);
			return fds[0];
		}
		else
		{
			close(fds[0]);
			return fds[1];
		}
	}
}
