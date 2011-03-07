#include <stdbool.h>

void gui_init(const char *appname);
bool gui_update();
void gui_quit();

extern int render_octree;
