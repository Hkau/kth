#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char buffer[128];
char *flags[16]; // space for argument pointers
bool shell_exit = false;

bool run(char *str)
{
	flags[0] = strtok(str, " \n");
	size_t i;
	for(i = 1; flags[i-1] != NULL && i < 15; ++i)
	{
		flags[i] = strtok(NULL, " \n");
	}
	flags[i] = NULL;
	for(i = 0; flags[i] != NULL; ++i)
		puts(flags[i]);
	if(flags[0] == NULL)
		return false;
	// exit command
	if(strcmp(flags[0], "exit") == 0)
		shell_exit = true;
	
	return true;
}

int main()
{
	while(true)
	{
		fputs("> ", stdout);
		char * foo = fgets(buffer, sizeof(buffer), stdin);

		if(foo == NULL || *foo == '\0' || strchr(foo, EOF) != NULL)
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

