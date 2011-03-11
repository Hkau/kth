#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define COUNTER_TAG 1
#define MESSAGE_TAG 0

#define MESSAGE_DONE 42
#define MESSAGE_ACCEPT 1
#define MESSAGE_REFUSE 0

MPI_Status status;

int n;
int *pref;

int size, rank;

int msg_num = 1;

// Dialog from a thread. Will add who's saying it and 
//#define line_printf(...) 
#define line_printf(...) do { printf("%s %d: #%02d> ", rank & 1 ? "Ms." : "Mr.", (rank/2), msg_num++); printf(__VA_ARGS__); fflush(stdout);} while(0)


// helps us write 1st, 2nd, 3rd, 4th... properly
const char *abbrev(int i)
{
	switch(i)
	{
		case 1:
			return "st";
		case 2:
			return "nd";
		case 3:
			return "rd";
		default:
			return "th";
	}
}

void man()
{
	int msg = 0;

	int has_gal = 0;

	int i;
	for(i = 0; i < n; ++i)
	{
		line_printf("I think I'll ask Ms. %d out.\n", i);

		// Send proposal to preferred woman previously not rejected from.
		MPI_Send(&msg, 1, MPI_INTEGER, pref[i]*2 + 1, pref[i]*2 + 1, MPI_COMM_WORLD);
		MPI_Recv(&msg, 1, MPI_INTEGER, pref[i]*2 + 1, rank, MPI_COMM_WORLD, &status);

		// If refused keep on looking. One will eventually accept you, you handsome man.
		if(msg == MESSAGE_REFUSE)
			continue;

		line_printf("Ms. %d accepted my proposal! <3\n", pref[i]);

		// Was this my first girl?
		if(has_gal == 0)
			// Let counter know.
			MPI_Send(&msg, 1, MPI_INTEGER, size-1, size-1, MPI_COMM_WORLD);
		has_gal = 1;

		// Recieve either message saying that we're done or being dumped.
		MPI_Recv(&msg, 1, MPI_INTEGER, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &status);

		switch(msg)
		{
			case MESSAGE_DONE:
				line_printf("I'll get married to my %d%s favorite woman Ms. %d.\n", i+1, abbrev(i+1), pref[i]);
				return;
			case MESSAGE_REFUSE:
				line_printf("I can't believe Ms. %d dumped me! :(\n", i);
				// Keep on looking!
				break;
			default:
				// Doesn't happen, good to have here if something would change.
				line_printf("THIS SHOULD NEVER HAPPEN! (%d sent by %d) :(\n", msg, status.MPI_SOURCE);
				break;
		}
	}
}

void woman()
{
	int msg = 0;

	// Recieve first proposal. Guaranteed to be proposed by some guy eventually.
	MPI_Recv(&msg, 1, MPI_INTEGER, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &status);

	// Accept it and mark that man as current favorite proposal.
	int fav = status.MPI_SOURCE / 2;
	msg = MESSAGE_ACCEPT;
	MPI_Send(&msg, 1, MPI_INTEGER, status.MPI_SOURCE, status.MPI_SOURCE, MPI_COMM_WORLD);

	// Send signal to counter thread that we've found someone.
	MPI_Send(&msg, 1, MPI_INTEGER, size-1, size-1, MPI_COMM_WORLD);

	while(1)
	{
		// Recieve a note. either that you're done or got a new proposal from someone else.
		MPI_Recv(&msg, 1, MPI_INTEGER, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &status);

		if(msg == MESSAGE_DONE)
		{
			line_printf("Perfect! I'm getting married to my %d%s favorite man Mr. %d.\n", pref[fav]+1, abbrev(pref[fav]+1), fav);
			return;
		}
		
		int num = status.MPI_SOURCE / 2;
		msg = MESSAGE_REFUSE;

		// Do we prefer this new preposal over our old one? (less = better)
		if(pref[num] < pref[fav])
		{
			// Break up with old guy, I like this guy better.
			MPI_Send(&msg, 1, MPI_INTEGER, fav*2, fav*2, MPI_COMM_WORLD);

			line_printf("Mr. %d asked me out, and he's way more handsome than Mr. %d.\n", num, fav);

			// Accept new guy's proposal.
			msg = MESSAGE_ACCEPT;
			MPI_Send(&msg, 1, MPI_INTEGER, num*2, num*2, MPI_COMM_WORLD);

			fav = num;
		}
		else
			// Refuse him, I like my guy better.
			MPI_Send(&msg, 1, MPI_INTEGER, num*2, num*2, MPI_COMM_WORLD);
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	n = (size-1) / 2;

	// each thread should get its own seed, to mix things up.
	// outcome will be the same, though that doesn't matter
	srand((rank+1) * time(NULL));

	// size-1 is the counting thread.
	if(rank == size-1)
	{
		int left = 2*n;
		int msg;
		while(left > 0)
		{
			// Recieve an "I'm done" status from everyone. Though they're not done,
			// instantly they will be when everyone's been matched up with someone.
			MPI_Recv(&msg, 1, MPI_INTEGER, MPI_ANY_SOURCE, size-1, MPI_COMM_WORLD, &status);

			printf("Cupid: Looks like %s %d found his first partner! %d people left...\n", status.MPI_SOURCE & 1 ? "Mrs." : "Mr.",  status.MPI_SOURCE/2, --left);
			fflush(stdout);
		}

		msg = MESSAGE_DONE;
		int i;

		for(i = 0; i < size-1; ++i)
		{
			// Let everyone know that execution is done.
			MPI_Send(&msg, 1, MPI_INTEGER, i, i, MPI_COMM_WORLD);
		}

		printf("\nCupid: And they lived stably ever after.\n");
		fflush(stdout);
	}
	// Everything else is a man/woman
	else
	{
		// Create and preferences
		pref = malloc(n * sizeof(int));
		int i;
		for(i = 0; i < n; ++i)
			pref[i] = i;
		//shuffle preferences
		for(i = n-1; i >= 0; --i)
		{
			int idx = rand() % n;

			int tmp = pref[idx];
			pref[idx] = pref[i];
			pref[i] = tmp;
		}

		// Odd number = woman, else man.
		if(rank & 1)
			woman();
		else
			man();
	}

	// Finished.
	MPI_Finalize();

	return 0;
}

