#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void	close_fds(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

int picoshell(char **cmds[])
{
	int fds[2];
	int last_fd = -1;
	pid_t pid;

	for (int i = 0; cmds[i]; i++)
	{
		if (cmds[i + 1] && pipe(fds) < 0)
			return 1;
		pid = fork();
		if (pid < 0)
		{
			if (cmds[i + 1])
				close_fds(fds);
			return 1;
		}
		if (pid == 0)
		{
			if (last_fd != -1)
			{
				if (dup2(last_fd, STDIN_FILENO) < 0)
					exit(1);
				close(last_fd);
			}
			if (cmds[i + 1])
			{
				if (dup2(fds[1], STDOUT_FILENO) < 0)
					exit(1);
				close_fds(fds);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (last_fd != -1)
			close(last_fd);
		if (cmds[i + 1])
		{
			close(fds[1]);
			last_fd = fds[0];
		}
	}
	
	while (wait(NULL) > 0)
		;
	return 0;
}
