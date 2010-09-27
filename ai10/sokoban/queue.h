#include <vector>

#ifndef QUEUE_H
#define QUEUE_H

template <class T>
class Queue
{
	const static unsigned int HASH_BITS = 16;
	const static unsigned int HASH_SIZE = (1<<HASH_BITS);
	const static unsigned int HASH_MASK = (1<<HASH_BITS)-1;
	std::vector<std::vector<T> > levels;
	std::vector<std::vector<T> > visited;
	unsigned int min_level;
	unsigned int max_level;
	unsigned int capacity;
	unsigned int count;

public:
	Queue() :
		visited(HASH_SIZE),
		min_level(0),
		max_level(0),
		capacity(16),
		count(0)
	{
		levels.resize(capacity);
	}
	void Push(const T &a, unsigned int level)
	{
		while (capacity <= level)
		{
			capacity <<= 1; // dubblera
			levels.resize(capacity);
		}

		// Check for duplicates
		unsigned int hash = Hash(a) & HASH_MASK;
		typeof(visited[hash].begin()) it = visited[hash].begin();
		typeof(visited[hash].end()) end = visited[hash].end();
		//std::vector<T>::const_iterator it = visited[hash].begin();
		//std::vector<T>::const_iterator end = visited[hash].end();
		for (; it != end; ++it)
			if (Equal(*it, a))
				return;

		visited[hash].push_back(a);

		// Add new state
		levels[level].push_back(a);
		++count;

		// Uppdatera bounds
		if (max_level < level)
			max_level = level;
		if (min_level > level)
			min_level = level;
	}

	T Pop()
	{
		// Prova om det finns något på min_level
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

		T tmp = levels[min_level][size-1];
		levels[min_level].pop_back();
		--count;
		return tmp;
	}

	unsigned int Size()
	{
		return count;
	}
};
#endif
