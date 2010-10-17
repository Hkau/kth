#ifndef HASHQUEUE_H
#define HASHQUEUE_H

#include <vector>
#include <deque>

template <class T>
class HashQueue
{
	const static unsigned int HASH_BITS = 21; // 2^21 == 2 097 152
	const static unsigned int HASH_SIZE = (1<<HASH_BITS);
	const static unsigned int HASH_MASK = (1<<HASH_BITS)-1;
	std::vector<std::deque<T> > levels;
	std::deque<T> early_queue;
	unsigned int early_left;
	std::vector<T> *visited;
	unsigned int min_level;
	unsigned int max_level;
	unsigned int capacity;
	unsigned int count;
	unsigned int insertion_count;

	public:
	HashQueue() :
		early_left(32),
		visited(new std::vector<T>[HASH_SIZE]),
		min_level(0),
		max_level(0),
		capacity(16),
		count(0),
		insertion_count(0)
	{
		levels.resize(capacity);
	}

	~HashQueue()
	{
		delete [] visited;
		visited = 0;
	}

	bool HasBeenPushed(const T &a) const
	{
		unsigned int hash = Hash(a) & HASH_MASK;
		typeof(visited[hash].begin()) it = visited[hash].begin();
		typeof(visited[hash].end()) end = visited[hash].end();
		for (; it != end; ++it)
			if (Equal(*it, a))
				return true;
		return false;
	}

	bool Push(const T &a, unsigned int level)
	{
		//assert(level == unsigned(-1));
		if(level == unsigned(-1))
			return false;

		// Check for duplicates
		unsigned int hash = Hash(a) & HASH_MASK;
		typeof(visited[hash].begin()) it = visited[hash].begin();
		typeof(visited[hash].end()) end = visited[hash].end();
		for (; it != end; ++it)
			if (Equal(*it, a))
				return false;

		visited[hash].push_back(a);
		++insertion_count;

		if(early_left != 0)
		{
			--early_left;
			early_queue.push_back(a);
			++count;
			return true;
		}

		if (capacity <= level)
		{
			do {
				capacity <<= 1; // dubblera
			} while (capacity <= level);

			levels.resize(capacity);
		}

		// Add new state
		levels[level].push_back(a);
		++count;

		// Uppdatera bounds
		if (max_level < level)
			max_level = level;
		if (min_level > level)
			min_level = level;
		return true;
	}

	T Pop()
	{
		if(!early_queue.empty())
		{
			T front = early_queue.front();
			early_queue.pop_front();
			--count;
			return front;
		}
		// See if there is anything on min_level
		unsigned int size = levels[min_level].size();
		while (size == 0)
		{
			++min_level;
			if (min_level > max_level)
			{
				// TODO: throw empty queue exception or similar
				return 0;
			}

			size = levels[min_level].size();
		}
#define POP_FRONT_FIRST
#ifdef POP_FRONT_FIRST
		// Remove first element
		T tmp = levels[min_level].front();
		levels[min_level].pop_front();
		--count;
		return tmp;
#else
#ifdef POP_BACK_FIRST
		// Remove last element
		T tmp = levels[min_level].back();
		levels[min_level].pop_back();
		--count;
		return tmp;
#else
		// Remove random element
		unsigned int r = rand()%size;
		T tmp = levels[min_level][r];
		levels[min_level][r] = levels[min_level][size-1];
		levels[min_level].pop_back();
		--count;
#endif
#endif
		return tmp;
	}

	unsigned int Size() const
	{
		return count;
	}

	unsigned int InsertionCount() const
	{
		return insertion_count;
	}
};
#endif
