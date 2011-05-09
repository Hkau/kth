#include "malloc.h"

#include <string.h>
#include <unistd.h>

#ifndef STRATEGY
#error STRATEGY must be defined.
#endif

#if STRATEGY < 1 || STRATEGY > 4
#error STRATEGY must contain a value 1-4.
#endif


typedef double alignment_type;

typedef union header
{
	struct {
		union header *next;
		size_t size;
	} s;
	alignment_type fake; /* force alignment of blocks */
} header;

header base = { .s = {&base, 0}};
header *freep = &base;

#define nalloc 1024 /* minimum amount of units to allocate each time */

header *morecore(size_t units)
{
	char *cp;
	header *hdr;

	if(units < nalloc)
		units = nalloc;

	cp = (char *) sbrk(units * sizeof(header));
	if(cp == (char *) -1)
		return NULL;
	hdr = (header *) cp;
	hdr->s.size = units;
	/* free((void *)(hdr + 1)); */ /* removed because quicklist's blocks have to be split and fixed outside */
	return hdr;
}

#if STRATEGY == 4

#ifndef NRQUICKLISTS
#define NRQUICKLISTS 4
#endif

header *quicklist[NRQUICKLISTS] = {0};

#if (4 << NRQUICKLISTS) < nalloc
#undef nalloc
#define nalloc (4 << NRQUICKLISTS)
#endif

#endif

void *malloc(size_t size)
{
	header *p, *prevp;
	size_t units;

	if(size == 0)
		return NULL;

	units = (size + sizeof(header)-1) / sizeof(header) + 1; /* get enough units, rounded up.*/

#if STRATEGY == 4
	if(units <= (4 << NRQUICKLISTS)) /* is the number of units small enough to take from a quick list? */
	{
		int list;
		size_t size, i;
		for(list = 0; units > (8 << list); ++list); /* find which list this block should be taken from */

		if(quicklist[list] != NULL) /* if there are blocks, use them */
		{
			header* hdr = quicklist[list];
			quicklist[list] = hdr->s.next;
			return (void *) (hdr+1);
		}
		size = 8 << list;
		/* else make new ones */
		header *more = morecore(nalloc);
		for(i = 0; i < nalloc; i += size)
		{
			more[i].s.size = size;
			free((void *) &more[i+1]);
		}

		header* hdr = quicklist[list];
		quicklist[list] = hdr->s.next;
		return (void *) (hdr+1);
	}
#endif
	prevp = freep;
	for(p = prevp->s.next;; prevp = p, p = p->s.next)
	{
		if(p->s.size >= units)
		{
			if(p->s.size == units)
				/* unlink nothing left to store */
				prevp->s.next = p->s.next;
			else
			{
				/* allocate tail, keep present block */
				p->s.size -= units;
				p += p->s.size;
				p->s.size = units;
			}
			freep = prevp;
			return (void *)(p+1);
		}
		if(p == freep)
		{
			if((p = morecore(units)) == NULL)
				return NULL;
			free((void *)(p + 1));
		}
	}
}

void *realloc(void *ptr, size_t size)
{
	header *hdr;
	size_t old_size;
	void *new_ptr;

	if(ptr == NULL)
		return malloc(size);

	if(size == 0)
	{
		free(ptr);
		return NULL;
	}

	hdr = ((header *)ptr) - 1;
	old_size = (hdr->s.size-1) * sizeof(header);
	if(old_size > size)
		old_size = size;

	new_ptr = malloc(size);
	if(new_ptr == NULL)
		return NULL;

	memcpy(new_ptr, ptr, old_size);
	free(ptr);

	return new_ptr;
}

void free(void *ptr)
{
	header *bp, *p;
	if(ptr == NULL)
		return;

	bp = (header *)ptr - 1;    /* point to block header */

#if STRATEGY == 4
	if(bp->s.size <= (4 << NRQUICKLISTS)) /* is the number of units small enough to take from a quick list? */
	{
		int list;
		for(list = 0; bp->s.size > (8 << list); ++list); /* find which list this block was taken from */

		bp->s.next = quicklist[list]; /* put it back */
		quicklist[list] = bp;
		return;
	}
#endif

	for (p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
		if (p >= p->s.next && (bp > p || bp < p->s.next))
			break; /* freed block at start or end of arena */

	if (bp + bp->s.size == p->s.next) {    /* join to upper nbr */
		bp->s.size += p->s.next->s.size;
		bp->s.next = p->s.next->s.next;
	} else
		bp->s.next = p->s.next;
	if (p + p->s.size == bp) {            /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.next = bp->s.next;
	} else
		p->s.next = bp;

	freep = p;
}

