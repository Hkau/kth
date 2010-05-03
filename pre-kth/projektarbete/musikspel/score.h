#ifndef SCORE_H
#define SCORE_H

#include "types.h"
#include "controller.h"

int score_init();

void score_time(u32 time);

u32 score_points();

u32 score_combo();

u32 score_max_combo();

void score_event(controller_event_t event);

#endif
