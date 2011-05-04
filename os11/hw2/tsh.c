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

bool shell_exit = false;

pid_t fg_pid = -1;

void exited(pid_t pid)
{
	printf("tsh: [%d] terminated\n", pid);
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

// myhandler is responsible for cleaning up zombie processes
// will report background processes as terminated if they were
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
			if(pid == fg_pid)
				continue;
			if(WIFEXITED(signal) || WIFSIGNALED(signal))
				exited(pid);
			// stop timer for pid;
		}
		return;
	}
}

// Parse a command and execute it
bool run(char *str)
{
	const char divider[] = " \n\t"; // Divider to split the command by space, \n or tab.
	char *args[16]; // Space for argument pointers

	// Read first token
	args[0] = strtok(str, divider);

	// Abort if this was an empty command
	if(args[0] == NULL)
		return false;

	// Test for exit command
	if(strcmp(args[0], "exit") == 0)
	{
		// Decide to exit
		shell_exit = true;
		return true;
	}

	// Test for cd
	if(strcmp(args[0], "cd") == 0)
	{
		// Read the next token
		args[1] = strtok(NULL, divider);

		// If there is no more tokens
		if(args[1] == NULL)
		{
			// Go to home directory
			char *home = getenv("HOME");
			if(home != NULL)
				cd(home);
			return true;
		}

		// Otherwise, try to change directory
		cd(args[1]);
		return true;
	}

	// This command was not a built-in command, try to execute it
	// Parse the rest of the command
	size_t i;
	for(i = 1; i < 15; ++i)
	{
		args[i] = strtok(NULL, divider);
		if (args[i] == NULL)
			break;
	}

	// Mark the end as NULL
	args[i] = NULL;

	// Decide if this command should run as a background process or not
	// The last character of the last argument decides
	bool bg = false;
	char * last_char = &( args[i-1][strlen(args[i-1])-1] );
	if( *last_char == '&')
	{
		bg = true;			  // Set background
		*last_char = '\0';	   // Remove ampersand
		if(*args[i-1] == '\0')  // Remove last argument if it only consisted of the ampersand
			args[i-1] = NULL;
		if(args[0] == NULL)	 // Abort if the whole command was only an ampersand
			return false;
	}

	// Create a pipe if this is a background process
	int pfd[2];
	if(bg)
		pipe(pfd);

	// Retrieve time
	struct timeval fgstart, fgend;
	gettimeofday(&fgstart, NULL);
	pid_t pid;

	#ifndef USE_POLL
	sigset_t sigchld_set;
	#endif
	if(!bg)
	{
		// If we use interrupts, disable them temporarily
		#ifndef USE_POLL
		sigemptyset(&sigchld_set);
		sigaddset(&sigchld_set, SIGCHLD);
		sigprocmask(SIG_BLOCK, &sigchld_set, NULL);
		#endif
		fg_pid = pid = fork();
	}
	else
		pid = fork();

	if(pid == 0)
	{
		// This is the child process
		if(bg)
		{
			dup2(pfd[STDIN_FILENO], STDIN_FILENO);
			close(pfd[0]);
			close(pfd[1]);
		}
		execvp(args[0], args);

		// This code is unreachable if exec was successful
		fprintf(stderr, "tsh: %s: command not found\n", args[0]);
		exit(-1);
	}
	else if(pid == -1)
	{
		// Something went wrong
		fputs("tsh: Could not fork\n", stderr);
	}
	if(bg)
	{
		// Close pipes
		close(pfd[0]);
		close(pfd[1]);
	}
	else
	{
		// If this is the foreground process, wait for it to terminate
		int status;
		waitpid(fg_pid, &status, 0);
		gettimeofday(&fgend, NULL);
		long ms = (fgend.tv_sec - fgstart.tv_sec) * 1000 + (fgend.tv_usec - fgstart.tv_usec) / 1000;
		fprintf(stderr, "tsh: %ld ms.\n", ms);

		// Unblock SIGCHLD after foreground process finishes, and clean up any zombies.
		#ifndef USE_POLL
		sigprocmask(SIG_UNBLOCK, &sigchld_set, NULL);
		myhandler(SIGCHLD); // Catch any zombies whose signal was blocked.
		#endif
	}

	return true;
}

int main()
{
	// Setup signal handlers for SIGINT and SIGCHLD
	struct sigaction act = {0};
	act.sa_handler = &myhandler;

	if ( 0 != sigaction(SIGINT, &act, 0) )
		perror("sigaction failed\n");

	#ifndef USE_POLL
	if ( 0 != sigaction(SIGCHLD, &act, NULL) )
		perror("sigaction failed\n");
	#endif

	// Main loop
	char buffer[128];
	while(true)
	{
		char *foo;
		// Read input, retry if interrupted
		while(true)
		{
			// Get current working directory
			char *cwd = getcwd(buffer, sizeof(buffer));

			// Print prompt
			if(cwd != NULL)
				fputs(cwd, stdout);
			fputs("$ ", stdout);

			// Reset errno
			errno = 0;

			// Read input
			foo = fgets(buffer, sizeof(buffer), stdin);

			// Get satisfied if successful
			if(foo != NULL || errno != EINTR)
				break;

			// Try again on a new line
			putchar('\n');

			#ifdef USE_POLL
			// See if there are any messages from other processes and handle them
			myhandler(SIGCHLD); // is ok, handler doesn't require to be able to wait for one
			#endif
		}

		#ifdef USE_POLL
		// See if there are any messages from other processes and handle them
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

