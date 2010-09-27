#ifndef __UNITTEST_H
#define __UNITTEST_H

#include <cxxtest/TestSuite.h>

#include "queue.h"
#include "lookup3.h"
#include "levelfromserver.h"
#include <string.h>

//
// A simple test suite: Just inherit CxxTest::TestSuite and write tests!
//

uint32_t Hash(const char*str)
{
	return hashlittle( (const void*) str, (size_t)strlen(str), 0 );
}

bool Equal(const char*a, const char*b)
{
	return strcmp(a,b) == 0;
}

class UnitTest : public CxxTest::TestSuite
{
public:
	void testQueue()
	{
		Queue<char *> queue;

		queue.Push("ett", 3);
		queue.Push("två", 1);
		queue.Push("tre", 2);
		queue.Push("fyra", 4);
		queue.Push("fem", 4);
		queue.Push("sex", 2);

		TS_ASSERT_EQUALS( "två", queue.Pop() );
		TS_ASSERT_EQUALS( "sex", queue.Pop() );
		TS_ASSERT_EQUALS( "tre", queue.Pop() );
		TS_ASSERT_EQUALS( "ett", queue.Pop() );
		TS_ASSERT_EQUALS( "fem", queue.Pop() );
		TS_ASSERT_EQUALS( "fyra", queue.Pop() );
	}

	void testQueueVisited()
	{
		Queue<char *> queue;

		queue.Push("ett", 3);
		queue.Push("två", 1);
		queue.Push("tre", 2);
		queue.Push("fyra", 4);
		queue.Push("fem", 4);
		queue.Push("sex", 2);
		queue.Push("ett", 3);
		queue.Push("två", 1);
		queue.Push("tre", 2);
		queue.Push("fyra", 4);
		queue.Push("fem", 4);
		queue.Push("sex", 2);

		TS_ASSERT_EQUALS( 6, queue.Size() );

		TS_ASSERT_EQUALS( "två", queue.Pop() );
		TS_ASSERT_EQUALS( "sex", queue.Pop() );
		TS_ASSERT_EQUALS( "tre", queue.Pop() );
		TS_ASSERT_EQUALS( "ett", queue.Pop() );
		TS_ASSERT_EQUALS( "fem", queue.Pop() );
		TS_ASSERT_EQUALS( "fyra", queue.Pop() );

		TS_ASSERT_EQUALS( 0, queue.Size() );

		queue.Push("ett", 3);
		queue.Push("två", 1);
		queue.Push("tre", 2);
		queue.Push("fyra", 4);
		queue.Push("fem", 4);
		queue.Push("sex", 2);

		TS_ASSERT_EQUALS( 0, queue.Size() );
	}

	void testLevelFromServer()
	{
		LevelFromServer server("cvap103.nada.kth.se");
		const char*level = server.getLevel(1);

		TS_ASSERT_EQUALS(strcmp(level,
"########\n\
#   # .#\n\
#   $$.#\n\
####   #\n\
####@ ##\n\
########\n"), 0);

		//SokobanLevel sokobanlevel(level);
		//sokobanlevel.print();

		char *MYSOL="U R R D U U L D L L U L L D R R R R L D D R U R U D L L U R";

		const char * answer = server.sendSolution(MYSOL);

		TS_ASSERT_EQUALS(strcmp(answer, "Good solution\n"), 0);
	}
};


#endif // __UNITTEST_H

