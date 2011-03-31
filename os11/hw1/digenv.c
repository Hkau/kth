/*
	Large main program comment.

	Fill me.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <signal.h>

int num_proc;

// closes open pipe file descriptors
void closepfd(int pfd[][2])
{
	int ret;
	for(int i = 0; i < num_proc-1; ++i)
	{
		ret = close(pfd[i][0]);
		if(ret == -1)
		{
			perror("close()");
			exit(-1);
		}

		ret = close(pfd[i][1]);
		if(ret == -1)
		{
			perror("close()");
			exit(-1);
		}
	}
}

// closes open pipe file descriptors and kills all processes belonging to the process group
void closekill(int pfd[][2], const char *str)
{
	closepfd(pfd);
	fputs(str, stderr);
	fputc('\n', stderr);
	kill(0, SIGKILL);
}

// executes 'prog' with argument list 'args' with stdin and stdout redirected to 'fd_stdin'/'fd_stdout'
int forkexecv(int pfd[][2], int fd_stdin, int fd_stdout, char *prog, char* args[])
{
	int pid = fork();
	if(pid == -1)
	{
		perror("fork()");
		return -1;
	}
	if(pid == 0)
	{
		int ret;
		ret = dup2(fd_stdin, STDIN_FILENO);
		if(ret == -1)
		{
			perror("dup2()");
			exit(-1);
		}

		ret = dup2(fd_stdout, STDOUT_FILENO);
		if(ret == -1)
		{
			perror("dup2()");
			exit(-1);
		}

		closepfd(pfd);

		ret = execvp(prog, args);
		perror("execlp()");
		exit(-1);
	}
	return 0;
}

// executes 'prog' with stdin/stdout redirected, with no additional arguments
int forkexec(int pfd[][2], int fd_stdin, int fd_stdout, char *prog)
{
	char *args[2] = {prog, (char *) NULL};
	return forkexecv(pfd, fd_stdin, fd_stdout, prog, args);
}

int main(int argc, char *argv[])
{
	char *pager = getenv("PAGER");

	if(pager == NULL)
		pager = "less";

	num_proc = 3 + (argc > 1);

	int pfd[num_proc-1][2];

	int ret;
	for(int i = 0; i < num_proc-1; ++i)
	{
		ret = pipe(pfd[i]);

		if(ret == -1)
		{
			perror("pipe()");
			exit(-1);
		}
	}

	// printenv | grep parameterlista | sort | less
	if(forkexec(pfd, STDIN_FILENO, pfd[0][1], "printenv"))
	{
		closekill(pfd, "couldn't start printenv");
		kill(0, SIGKILL);
		return -1;
	}
	int start_grep = argc > 1;
	// start grep if there are grep arguments
	if(start_grep)
	{
		char **subargs = malloc((argc+1)*sizeof(char *));

		memcpy(&subargs[1], &argv[1], (argc-1)*sizeof(char *));

		subargs[argc] = NULL;
		subargs[0] = "grep";

		if(forkexecv(pfd, pfd[0][0], pfd[1][1], "grep", subargs))
		{
			closekill(pfd, "couldn't start printenv");
			return -1;
		}

	}
	if(forkexec(pfd, pfd[start_grep][0], pfd[start_grep+1][1], "sort"))
	{
		closekill(pfd, "couldn't start sort");
		return -1;
	}
	if(forkexec(pfd, pfd[start_grep+1][0], STDOUT_FILENO, pager))
	{
		closekill(pfd, "couldn't start PAGER");
		return -1;
	}

	closepfd(pfd);

	int status;
	for(int i = 0; i < num_proc; ++i)
	{
		do{
			wait(&status);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));

		if(WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
		{
			fprintf(stderr, "error, process exited abnormally.\n");
			kill(0, SIGKILL);
			return -1;
		}
	}

	return 0;
}

