#ifndef INTERFACE_H
#define INTERFACE_H

#include "controller.h"

int interface_init();

void interface_event(controller_event_t event);

void interface_draw();

void interface_quit();

#endif

