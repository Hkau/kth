#include <iostream>

#include "levelfromserver.h"
#include "sokobanlevel.h"
#include "sokobanstate.h"
#include "queue.h"
#include <cstring>
#include <cstdlib>

int levelfromserver(const char*address, int boardnr);

#include "lookup3.h"

uint32_t Hash(const char*str)
{
	return hashlittle( (const void*) str, (size_t)strlen(str), 0 );
}

int main(int argc, char *argv[])
{
	int boardnr = 0;
	if(argc == 2)
		boardnr = atoi(argv[1]);
	else
	{
		printf("Enter boardnr: ");
		scanf("%d", &boardnr);
	}
	LevelFromServer server("cvap103.nada.kth.se");
	const char *str = server.getLevel(boardnr);

	SokobanLevel level(str);
	level.print();

	const char *MYSOL="U R R D U U L D L L U L L D R R R R L D D R U R U D L L U R";

//	const char * answer = server.sendSolution(MYSOL);

//	puts(answer);

	Queue<SokobanState*> queue;

	SokobanState *first_state = new SokobanState(level);
	queue.Push(first_state, first_state->Cost());

	while (queue.Size() > 0)
	{
		SokobanState *state = queue.Pop();

		// Generera alla möjliga drag härifrån
		// Om giltig lösning så är vi klara, skriv ut lösningen
		// Annars lägg in dem i kön
		//state.add_children(queue);

		state->SpawnChildren(queue);

		if (state->Cost() == 0)
		{
			std::string moves = state->Moves();
			printf("answer: %s\n", moves.c_str());
			const char * answer = server.sendSolution(moves.c_str());
			puts(answer);
			return 0;
		}
	}

	printf("queue is empty\n");

	return 0;
}
