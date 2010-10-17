#include <iostream>
#include <cstring>
#include <cstdlib>

#include "levelfromserver.h"
std::string levelfromfile(const char *filename);
#include "sokobanlevel.h"
#include "sokobanstate.h"
#include "hashqueue.h"

int levelfromserver(const char*address, int boardnr);

#include "lookup3.h"

uint32_t Hash(const char*str)
{
	return hashlittle( (const void*) str, (size_t)strlen(str), 0 );
}

int main(int argc, char *argv[])
{
	int boardnr = 0;
	LevelFromServer * server = NULL;
	if(argc == 2)
		boardnr = atoi(argv[1]);
	else
	{
		printf("Enter boardnr: ");
		scanf("%d", &boardnr);
	}
	std::string str;
	if (boardnr < 0)
	{
		char filename[64];
		sprintf(filename, "maps/screen.%d", -boardnr);
		str = levelfromfile(filename);
	}
	else
	{
		server = new LevelFromServer("cvap103.nada.kth.se");
		//server = new LevelFromServer("130.237.223.120");
		str = server->getLevel(boardnr);
	}

	printf("Preprocessing level..\n");
	SokobanLevel level(str.c_str());
	level.Print();

	printf("Creating queues...\n");

	HashQueue<SokobanState*> queue;

	SokobanState *first_state = new SokobanState(level);
	queue.Push(first_state, first_state->Cost());

	unsigned int expansioncount = 0;
	unsigned int generatedcount = 0;
	unsigned int messagelimit = 1024;

	printf("Begins expanding nodes...\n");

	std::vector<SokobanState*> moves_to_add;

	while (queue.Size() > 0)
	{
		SokobanState *state = queue.Pop();

		// If this state is a solution, generate solution string
		// Otherwise generate all possible moves from this state
		// and insert them into the queue
		if (state->IsSolution())
		{
			std::string moves = state->Moves();
			printf("\n**** BOARD #%d SOLUTION ****\n%s\n", boardnr, moves.c_str());
			if (server != NULL)
			{
				const char * answer = server->sendSolution(moves.c_str());
				puts(answer);
			}
			printf("expansion count: %d\n", expansioncount);
			printf("generated count: %d\n", generatedcount);
			printf("insertion count: %d\n", queue.InsertionCount());
			printf("duplicates factor: %.5f\n", 1.0-(queue.InsertionCount() / (float)generatedcount));
			printf("branching factor: %.2f\n", queue.InsertionCount() / (float)expansioncount);
			printf("scribbles created: %d\n", level.NumScribbles());
			fflush(stdout);
			delete server;
			server = NULL;
			return 0;
		}

		++expansioncount;
		moves_to_add.clear();
		state->SpawnChildren(moves_to_add);

		generatedcount += moves_to_add.size();
		for (unsigned int i = 0; i < moves_to_add.size(); ++i)
		{
			SokobanState *s = moves_to_add[i];
			if ( false == queue.Push(s, s->Cost()) )
				delete s;
		}

		if (expansioncount == messagelimit)
		{
			state->Print();
			printf("expansion count: %d\n", expansioncount);
			printf("generated count: %d\n", generatedcount);
			printf("insertion count: %d\n", queue.InsertionCount());
			printf("duplicates factor: %.5f\n", 1.0-(queue.InsertionCount() / (float)generatedcount));
			printf("branching factor: %.2f\n", queue.InsertionCount() / (float)expansioncount);
			printf("scribbles created: %d\n", level.NumScribbles());
			fflush(stdout);
			messagelimit <<= 1;
		}
	}

	printf("*** BOARD #%d FAILED ***: queue is empty\n", boardnr);
	printf("expansion count: %d\n", expansioncount);
	printf("generated count: %d\n", generatedcount);
	printf("insertion count: %d\n", queue.InsertionCount());
	printf("duplicates factor: %.5f\n", 1.0-(queue.InsertionCount() / (float)generatedcount));
	printf("branching factor: %.2f\n", queue.InsertionCount() / (float)expansioncount);
	printf("scribbles created: %d\n", level.NumScribbles());
	fflush(stdout);

	delete server;
	server = NULL;
	return 0;
}
