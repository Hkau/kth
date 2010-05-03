#include "SDL.h"

#include "types.h"

#include "interface.h"
#include "scene.h"
#include "score.h"

#include <GL/gl.h>
#include <GL/glu.h>

#define	TEMP_WINDOW_WIDTH	640
#define	TEMP_WINDOW_HEIGHT	480

extern float fps;

static float colors[5][3] = {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.2f, 1.0f}, {1.0f, 0.7f, 0.0f}};
static float colors_desaturated[5][3] = {{0.0f, 0.7f, 0.0f}, {0.7f, 0.0f, 0.0f}, {0.7f, 0.7f, 0.0f}, {0.0f, 0.0f, 0.7f}, {0.7f, 0.5f, 0.0f}};
static float black[3] = {0.0f, 0.0f, 0.0f};
static float gray[3] = {0.7f, 0.7f, 0.7f};
static float white[3] = {1.0f, 1.0f, 1.0f};

u32 current_time = 0;
#define ms_per_frame 1000 //temp

void set_title()
{
	char title[128];

	sprintf(title, "Score: %d; Combo: %d; Max combo: %d; FPS: %f;", score_points(), score_combo(), score_max_combo(), fps);
	SDL_WM_SetCaption(title, NULL);
}

void interface_time(u32 time)
{
	static u8 frames = 0;
	current_time = time;
	if(++frames>50)
	{
		set_title();
		frames = 0;
	}
}

void interface_event(controller_event_t event)
{
	/*if(event.type == BUTTON_STRUM)
		SDL_WM_SetCaption("strumlol", NULL);*/
	set_title();
}

void gl_init()
{
	glShadeModel( GL_SMOOTH );

	glClearColor( 0.9f, 0.9f, 0.9f, 0.0f );
	glClearDepth( 1.0f );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc( GL_LEQUAL );

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void gl_set_viewport(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, width, 0, height, -1, 1);

	/*GLdouble aspect_ratio = (GLdouble)height / (GLdouble)width;

	if (aspect_ratio >= 1.0)
		glFrustum(-1.0, 1.0, -aspect_ratio, aspect_ratio, 5.0, 60.0);
	else
		glFrustum(-1.0/aspect_ratio, 1.0/aspect_ratio, -1.0, 1.0, 5.0, 60.0);*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(2.0f);
}

static float *note_color;
static float *note_edge;

inline void draw_note()
{
	glColor3fv(note_color);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);

		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 7.0f);
		glVertex2f(40.0f, 7.0f);
		glVertex2f(40.0f, 0.0f);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3fv(note_edge);

	glBegin(GL_QUADS);

		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 7.0f);
		glVertex2f(40.0f, 7.0f);
		glVertex2f(40.0f, 0.0f);

	glEnd();
}

void interface_draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

		glTranslatef(10.0f, 10.0f, 0.0f);

		glColor3fv(gray);

		glBegin(GL_LINES);
			glVertex2f(40.0f, 0.0f);
			glVertex2f(40.0f, TEMP_WINDOW_HEIGHT);

			glVertex2f(80.0f, 0.0f);
			glVertex2f(80.0f, TEMP_WINDOW_HEIGHT);

			glVertex2f(120.0f, 0.0f);
			glVertex2f(120.0f, TEMP_WINDOW_HEIGHT);

			glVertex2f(160.0f, 0.0f);
			glVertex2f(160.0f, TEMP_WINDOW_HEIGHT);
		glEnd();

		#define player 0 //temp

		//draw notes
		note_edge = black;
		u8 i;
		for(i = 0; i < 5; ++i)
		{
			note_color = colors[i];
			u16 j;

			for(j = scene.current_note[player][i]; j < scene.channel[player][i].num_notes && scene.channel[player][i].note_list[j].timestamp < current_time || (scene.channel[player][i].note_list[j].timestamp - current_time < ms_per_frame); ++j)
			{
				glPushMatrix();

					glTranslatef(i*40.0f, TEMP_WINDOW_HEIGHT*(scene.channel[player][i].note_list[j].timestamp-current_time) / ms_per_frame, 0.0f);

					draw_note();

				glPopMatrix();
			}
		}

		// draw unheld buttons
		// note_edge = black;
		for(i=0; i < 5; ++i)
		{
			if(scene.channel[player][i].status == CHANNEL_HELD)
				continue;

			glPushMatrix();
				glTranslatef(i*40.0f, 0.0f, 0.0f);
				note_color = colors_desaturated[i];
				draw_note();
			glPopMatrix();
		}

		// draw held buttons
		// note_edge = white;
		for(i=0; i < 5; ++i)
		{
			if(scene.channel[player][i].status != CHANNEL_HELD)
				continue;

			glPushMatrix();
				glTranslatef(i*40.0f, 0.0f, 0.0f);
				note_color = colors[i];
				draw_note();
			glPopMatrix();
		}

		glColor3fv(black);

		glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, TEMP_WINDOW_HEIGHT);

			glVertex2f(200.0f, 0.0f);
			glVertex2f(200.0f, TEMP_WINDOW_HEIGHT);
		glEnd();

	glPopMatrix();

	SDL_GL_SwapBuffers();
}

int interface_init()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL init failed, lol, wonder why? D:\n");
		
		return -1;
	}

	/*#ifdef WIN32
	// disable stdio redirect
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
	#endif*/ // This somehow fucks up ogg decoding when not running from a console, todo compile SDL without stdio redirect

	SDL_JoystickEventState(SDL_ENABLE);	//Enable joystick event handling

	SDL_SetVideoMode(TEMP_WINDOW_WIDTH, TEMP_WINDOW_HEIGHT, 24, SDL_OPENGL | SDL_DOUBLEBUF);

	SDL_WM_SetCaption("? fps", NULL);

	gl_init();
	
	gl_set_viewport(TEMP_WINDOW_WIDTH, TEMP_WINDOW_HEIGHT);

	return 0;
}

void interface_quit()
{
	SDL_Quit();
}

