#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>


char buffer[128];
char *flags[16]; // space for argument pointers
bool shell_exit = false;

bool forking = false; // background process messages will be suppressed for the short instant when the parent is forking.
pid_t fg_pid = -1;

void exited(pid_t pid)
{
	printf("tsh: [%d] stopped\n", pid);
}

// chose not to go to HOME and properly report the errors instead.
// hope that's ok, 'cd' goes home as it should.
bool cd(char *path)
{
	if(chdir(path) == 0)
		return true;
	fprintf(stderr, "tsh: cd: %s: ", path);
	switch(errno)
	{
		case EACCES:
			fputs("Access denied\n", stderr);
			break;
		case ENOENT:
			fputs("File not found\n", stderr);
			break;
		case ENOTDIR:
			fputs("Not a directory\n", stderr);
			break;
		default:
			fputs("Unknown error\n", stderr);
			break;
	}
	return false;
}

bool run(char *str)
{
	flags[0] = strtok(str, " \n");

	if(flags[0] == NULL)
		return false;
	// exit command
	if(strcmp(flags[0], "exit") == 0)
	{
		shell_exit = true;
		return true;
	}

	if(strcmp(flags[0], "cd") == 0)
	{
		flags[1] = strtok(NULL, " \n");
		if(flags[1] == NULL)
		{
			char *home = getenv("HOME");
			if(home != NULL)
				cd(home);
			return true;
		}
		cd(flags[1]);
		return true;
	}

	size_t i;
	for(i = 1; flags[i-1] != NULL && i < 15; ++i)
	{
		flags[i] = strtok(NULL, " \n");
	}

	if(i == 15)
		flags[i] = NULL;
	else
		--i;

	bool bg = false;
	if(flags[i-1][strlen(flags[i-1])-1] == '&')
	{
		bg = true;
		flags[i-1][strlen(flags[i-1])-1] = '\0';
		if(*flags[i-1] == '\0')
			flags[i-1] = NULL;
		if(flags[0] == NULL)
			return false;
	}

	int pfd[2];
	if(bg)
		pipe(pfd);

	struct timeval fgstart, fgend;
	gettimeofday(&fgstart, NULL);
	pid_t pid;
	if(!bg)
	{
		forking = true;
		fg_pid = pid = fork();
		forking = false;
	}
	else
		pid = fork();

	if(pid == 0)
	{
		if(bg)
		{
			dup2(pfd[STDIN_FILENO], STDIN_FILENO);
			close(pfd[0]);
			close(pfd[1]);
		}
		execvp(flags[0], flags);
		// skriv ut nåt fel
		fprintf(stderr, "tsh: %s: command not found\n", flags[0]);
		exit(-1);
	}
	else if(pid == -1)
	{
		fputs("tsh: Could not fork\n", stderr);
	}
	if(bg)
	{
		close(pfd[0]);
		close(pfd[1]);
	}
	else
	{
		int status;
		waitpid(fg_pid, &status, 0);
		gettimeofday(&fgend, NULL);
		printf("tsh: %.0f ms.\n", ((float) fgend.tv_usec - fgstart.tv_usec)/1000.f);
	}

	return true;
}

void myhandler(int signal)
{
	if(signal == SIGCHLD)
	{
		int signal;
		while(true)
		{
			pid_t pid = waitpid(-1, &signal, WNOHANG);
			if(pid <= 0)
			{
				// no zombie left to wait for
				break;
			}
			if(pid == fg_pid || forking)
				continue;
			if(WIFEXITED(signal))
				exited(pid);
			// stop timer for pid;
		}
		return;
	}
	fprintf(stderr, "myhandler recieved signal %d\n", signal);
}

int main()
{
	struct sigaction act = {0};
	act.sa_handler = &myhandler;

	if ( 0 != sigaction(SIGINT, &act, 0) )
		perror("sigaction failed\n");

	#ifndef USE_POLL
	if ( 0 != sigaction(SIGCHLD, &act, NULL) )
		perror("sigaction failed\n");
	#endif

	while(true)
	{
		char *pwd = getcwd(buffer, sizeof(buffer));
		if(pwd != NULL)
			fputs(pwd, stdout);
		fputs("$ ", stdout);
		char *foo;
		do
		{
			errno = 0;
			foo = fgets(buffer, sizeof(buffer), stdin);
		}while(foo == NULL && errno == EINTR);
		#ifdef USE_POLL
		myhandler(SIGCHLD); // is ok, handler doesn't require to be able to wait for one
		#endif

		if(foo == NULL || *foo == '\0')
		{
			putchar('\n');
			break;
		}

		run(foo);

		if(shell_exit)
			break;
	}
	return 0;
}

