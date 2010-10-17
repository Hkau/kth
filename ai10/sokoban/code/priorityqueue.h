#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <queue>

template <class T>
class PriorityQueue
{
public:
    class Post
    {
    public:
        T elem;
        int priority;
        Post(const Post &p) :
            elem(p.elem),
            priority(p.priority)
        {
        }
        Post(const T &elem, int priority) :
            elem(elem),
            priority(priority)
        {
        }
        bool operator<(const Post &b) const
        {
            // Reverse sorting so minimum is poped first
            return priority > b.priority;
        }
    };
private:
	std::priority_queue<Post> q;
public:

	PriorityQueue()
	{

	}

	void Push(const T &a, int level)
	{
        q.push(Post(a,level));
	}

	T Pop()
	{
	    T tmp = q.top().elem;
		q.pop();
		return tmp;
	}

	bool Empty() const
	{
		return q.empty();
	}
};
#endif

