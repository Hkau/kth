#include "gui.h"
#include "nbody.h"

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>

#ifdef USE_OCTREE
	#include "octree.h"
#endif
int render_octree = 1;

int last_ms;
bool disabled = false;
void gui_init(const char *appname)
{
	if(num_steps > 0)
	{
		disabled = true;
		return;
	}
	SDL_Init(SDL_INIT_VIDEO);

	SDL_WM_SetCaption(appname, appname);


	SDL_Surface *screen;
	if((screen = SDL_SetVideoMode(0, 0, 24, SDL_DOUBLEBUF | SDL_OPENGL | SDL_FULLSCREEN)) == NULL)
	//if((screen = SDL_SetVideoMode(1024, 768, 24, SDL_DOUBLEBUF | SDL_OPENGL)) == NULL)
		exit(-1);


	//SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);

	/* init opengl/ */

	glViewport(0, 0, screen->w, screen->h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(0, screen->w, 0, screen->h, 0.1, 1000);
	// left, right, bottom, top, near, far
	GLfloat fov = 45.0f;
	GLfloat zNear = 0.1f;
	GLfloat zFar = 100000.0f;
	GLfloat aspect = (float)(screen->w)/(float)(screen->h);
	GLfloat fH = tan( (float)(fov / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );

	//glFrustum(-(screen->w >> 1), screen->w >> 1, -(screen->h >> 1), screen->h >> 1, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);
	
	last_ms = SDL_GetTicks();
}

int x = 0, y = 0;
float turbo = 1.f;

#ifdef USE_OCTREE
void draw_octree(octree *node)
{
	if(node->count == 0)
		return;

	// min point
	glVertex3f(node->min.x, node->min.y, node->min.z);
	glVertex3f(node->max.x, node->min.y, node->min.z);

	glVertex3f(node->min.x, node->min.y, node->min.z);
	glVertex3f(node->min.x, node->max.y, node->min.z);

	glVertex3f(node->min.x, node->min.y, node->min.z);
	glVertex3f(node->min.x, node->min.y, node->max.z);

	// max x point
	glVertex3f(node->max.x, node->min.y, node->min.z);
	glVertex3f(node->max.x, node->max.y, node->min.z);

	glVertex3f(node->max.x, node->min.y, node->min.z);
	glVertex3f(node->max.x, node->min.y, node->max.z);

	// max y point
	glVertex3f(node->min.x, node->max.y, node->min.z);
	glVertex3f(node->max.x, node->max.y, node->min.z);

	glVertex3f(node->min.x, node->max.y, node->min.z);
	glVertex3f(node->min.x, node->max.y, node->max.z);

	// max z point
	glVertex3f(node->min.x, node->min.y, node->max.z);
	glVertex3f(node->max.x, node->min.y, node->max.z);

	glVertex3f(node->min.x, node->min.y, node->max.z);
	glVertex3f(node->min.x, node->max.y, node->max.z);

	// max point
	glVertex3f(node->max.x, node->max.y, node->max.z);
	glVertex3f(node->min.x, node->max.y, node->max.z);

	glVertex3f(node->max.x, node->max.y, node->max.z);
	glVertex3f(node->max.x, node->min.y, node->max.z);

	glVertex3f(node->max.x, node->max.y, node->max.z);
	glVertex3f(node->max.x, node->max.y, node->min.z);

	if(node->children == NULL)
		return;
	int i;
	for(i = 0; i < 8; ++i)
		draw_octree(&node->children[i]);
}
#endif

bool gui_update()
{
	if(disabled)
		return true;

	while(true)
	{
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_q:
						case SDLK_ESCAPE:
							return false;
						case SDLK_TAB:
							turbo = 10.f;
							break;
						case SDLK_LCTRL:
						case SDLK_RCTRL:
							turbo = 0.f;
							break;
						case SDLK_LSHIFT:
						case SDLK_RSHIFT:
							turbo = 0.1f;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_TAB:
						case SDLK_LCTRL:
						case SDLK_RCTRL:
						case SDLK_LSHIFT:
						case SDLK_RSHIFT:
							turbo = 1.f;
							break;
						default:
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					x += event.motion.xrel;
					y += event.motion.yrel;
					break;
				default:
					break;
			}
		}

		if(SDL_GetTicks() - last_ms >= 10)
		{
			//printf("%d ms/frame\n", SDL_GetTicks() - last_ms);
			last_ms = SDL_GetTicks();
			break;
		}
	}

	// render

	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	float distance = 100;
	int i;
	for(i = 0; i < num_bodies; ++i)
	{
		if(abs(body[i].pos.x) > distance)
			distance = abs(body[i].pos.x);
		if(abs(body[i].pos.y) > distance)
			distance = abs(body[i].pos.y);
		if(abs(body[i].pos.z) > distance)
			distance = abs(body[i].pos.z);
	}
#ifdef USE_OCTREE
	if(render_octree)
		glTranslatef(0, 0, -2*bounds);
	else
		glTranslatef(0, 0, -2*distance);
#else
	glTranslatef(0, 0, -2*distance);
#endif
	glRotatef(y/20.0, 1, 0, 0);
	glRotatef(x/20.0, 0, 0, 1);

	// Draw grid
	glColor3f(0.15, 0.15, 0.15);
	glBegin(GL_LINES);

	for(i = -100; i <= 100; i+= 10)
	{
		glVertex3i(-100, i, 0);
		glVertex3i(100, i, 0);
		glVertex3i(i, -100, 0);
		glVertex3i(i, 100, 0);
	}

	glEnd();

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINES);

		glVertex3i(-2, 0, 0);
		glVertex3i(2, 0, 0);

		glVertex3i(0, -2, 0);
		glVertex3i(0, 2, 0);

		glVertex3i(0, 0, -2);
		glVertex3i(0, 0, 2);
	glEnd();

#ifdef USE_OCTREE
	if(render_octree)
	{
		glColor3f(0., .2, 0.);
		glBegin(GL_LINES);
		draw_octree(top);
		glEnd();
	}
#endif

	glColor3f(1, 1, 1);
	glPointSize(1);

	float max_mass = 0;
	for(i = 0; i < num_bodies; ++i)
	{
		if(abs(body[i].mass) > max_mass)
			max_mass = abs(body[i].mass);
	}

	for(i = 0; i < num_bodies; ++i)
	{
		if(body[i].mass < 0)
			glColor3f(-body[i].mass/max_mass, -body[i].mass/max_mass, 0);
		else
			glColor3f(body[i].mass/max_mass, body[i].mass/max_mass, body[i].mass/max_mass);

		glBegin(GL_POINTS);
			glVertex3f(body[i].pos.x, body[i].pos.y, body[i].pos.z);
		glEnd();
	}

	glFlush();
	SDL_GL_SwapBuffers();
	return true;
}

void gui_quit()
{
	if(disabled)
		return;
	SDL_Quit();
}

