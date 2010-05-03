#include <stdio.h>
#include "SDL.h"
#include <time.h>

int main (int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0 )
	{
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		return 1;
	}

	freopen("CON", "w", stdout); // reenables console stdout

	printf("Number of joysticks: %d\n", SDL_NumJoysticks());

	if(SDL_NumJoysticks() == 0)
		return 0;

	while(1)
	{
		unsigned int i;
		for(i = 0; i < SDL_NumJoysticks(); ++i)
		{
			static SDL_Event event;

			SDL_Joystick *joystick;

			joystick = SDL_JoystickOpen(i);

			SDL_JoystickUpdate();


			if(joystick)
			{
				int j = 0;
				printf("Joystick %d\n", i);

				printf("\tName: %s\n", SDL_JoystickName(i));

				printf("\tNumber of Axes: %d\n", SDL_JoystickNumAxes(joystick));
				for(j = 0; j < SDL_JoystickNumAxes(joystick); ++j)
					printf("\t\tAxis[%d]: %d\n", j, SDL_JoystickGetAxis(joystick, j));

				printf("\tNumber of Buttons: %d\n", SDL_JoystickNumButtons(joystick));
				for(j = 0; j < SDL_JoystickNumButtons(joystick); ++j)
					printf("\t\tButton[%d]: %d\n", j, SDL_JoystickGetButton(joystick, j));

				printf("\tNumber of Hats: %d\n", SDL_JoystickNumHats(joystick));
				for(j = 0; j < SDL_JoystickNumHats(joystick); ++j)
					printf("\t\tHat[%d]: %d\n", j, SDL_JoystickGetHat(joystick, j));

				SDL_JoystickClose(joystick);
			}
			else
				printf("Couldn't open Joystick %d\n", i);
		}

		// useless timestoppahcrap
		clock_t endwait;
		endwait = clock() + 250;
		while (clock() < endwait);
	}

	return 0;
}
