#include "score.h"

#include "scene.h"

u32 current_time;
u32 score;
u32 combo;
//u32 max_score;
u32 max_combo;

int abs (int i)
{
    if (i < 0)
        return -i;
    else
        return i;
}

#define player 0 //temp

#define scored_offset 100
int score_init()
{
	score = 0;
	combo = 0;
	//max_score=0;
	max_combo = 0;

	return 0;
}

u32 score_combo()
{
	return combo;
}
u32 score_max_combo()
{
	return max_combo;
}

u32 score_points()
{
	return score;
}

void score_time(u32 time)
{
	current_time = time;

	u8 i;
	
	for(i = 0; i < 5; ++i)
	{
		while((s32)current_time - (s32)scene.channel[player][i].note_list[scene.current_note[player][i]].timestamp > scored_offset)
		{
			if(scene.channel[player][i].note_list[scene.current_note[player][i]].note_flags ^ NOTE_HIT)
				combo = 0;
			++scene.current_note[player][i];
		}
		
	}
}

void score_event(controller_event_t event)
{
	if(event.type == BUTTON_STRUM)
	{
		u8 scoring = 0;
		u32 scoring_timestamp = 0;
		bool past_timestamp = false;
		u32 last_timestamp = 0;

		u32 i;

		//scene.current_note[player][i]

		for(i = 0; i < 5; ++i)
		{
			s32 j;
			for(j = scene.current_note[player][i]; abs((s32)current_time - (s32)scene.channel[player][i].note_list[j].timestamp) <= scored_offset; ++j)
			{
				if(!(scene.channel[player][i].note_list[j].note_flags & NOTE_HIT))
				{
					scene.channel[player][i].note_list[j].note_flags |= NOTE_HIT;

					if(scoring_timestamp == 0)
					{
						scoring_timestamp = scene.channel[player][i].note_list[j].timestamp;
					}
					else if(scoring_timestamp != scene.channel[player][i].note_list[j].timestamp)
					{
						if(scoring_timestamp < scene.channel[player][i].note_list[j].timestamp && scene.channel[player][i].note_list[j].timestamp > last_timestamp)
						{
							scoring_timestamp = scene.channel[player][i].note_list[j].timestamp;
							i = 0;
							j=scene.current_note[player][i]-1;
							
							break;
						}
					}

					++scoring;

					break;
				}
			}
		}

		if(scoring != 0)
		{
			score+=scoring;
			combo+=scoring;

			if(combo > max_combo)
				max_combo = combo;
		}
		else
			combo = 0;
	}

	else if(event.type == BUTTON_CHANNEL)
	{
		if(event.state == BUTTON_UP)
			scene.channel[player][event.button].status = CHANNEL_EMPTY;
		else
			scene.channel[player][event.button].status = CHANNEL_HELD;
	}
}
