#include "SDL.h"
#include <stdio.h>

#include "interface.h"
#include "xplorer.h"
#include "sound.h"
#include "scene.h"

joystick_t joystick;

float fps = 0.0f;

void send_controller_event(controller_event_t event)
{
	score_event(event);
	interface_event(event);
}

int main(int argc, char *argv[])
{
	if(interface_init() != 0 || sound_init() != 0)
		return -1;

	if(argc < 2)
	{
		printf("No input song directory\n");

		return -1;
	}

	if(FOF_load(argv[1]) != 0)
	{
		printf("Unable to open FOF song from folder.\n");

		return -1;
	}

	score_init();

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	SDL_JoystickOpen(0);

	controller_xplorer(&joystick);

	//Start game

	sound_play();
	
	u32 start_ticks = SDL_GetTicks();

	bool quit = false;

	while(!quit){
		static SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			controller_event_t controller_event;
			switch(event.type)
			{
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_q)
						quit=true;

					break;

				case SDL_QUIT:
					quit=true;
					break;

				case SDL_JOYBUTTONDOWN:
					send_controller_event(joystick.convert_button(event.jbutton.button, BUTTON_DOWN));
				
					break;

				case SDL_JOYBUTTONUP:
					send_controller_event(joystick.convert_button(event.jbutton.button, BUTTON_UP));
					break;
				
				case SDL_JOYHATMOTION:
					send_controller_event(joystick.convert_hat(event.jhat.hat, event.jhat.value));
					break;

				default:
					break;
			}
		}
		static unsigned int delta_ticks, ticks, cur_ticks;

		ticks = SDL_GetTicks();

		cur_ticks = SDL_GetTicks();
		delta_ticks = cur_ticks - ticks;
		ticks = cur_ticks;
		u32 game_ticks = ticks - start_ticks;
		interface_time(game_ticks);
		score_time(game_ticks);

		if(game_ticks > scene.length)
			break;

		SDL_Delay(5);

		interface_draw();

		static unsigned int frames = 0;
		static int t0 = 0;

		++frames;

		int t = SDL_GetTicks();
		if (t - t0 >= 1000)
		{
			//printf("%d frames in %d milliseconds = %g frames/second\n", Frames, (t - t0), Frames * 1000.0f / (t-t0));
			//char foo[256];

			fps = frames * 1000.0f / (t-t0);
			//sprintf(foo, "%s, %g fps", argv[1], frames * 1000.0f / (t-t0));

			//SDL_WM_SetCaption(foo, NULL);
			t0 = t;
			frames = 0;
		}
	}

	FOF_close();

	SDL_JoystickClose(0);

	interface_quit();

	sound_quit();

	return 0;
}
