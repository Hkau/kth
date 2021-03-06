#ifndef _brk_h_
#define _brk_h_


#ifndef	_UNISTD_H // USE WITH CAUTION brk() and sbrk() have been removed from the POSIX standard, most systems implement them but the parameter types may vary
extern int brk(void *);
extern void *sbrk(unsigned );
#endif

#ifndef __USE_BSD // backwards compatibility with BSD
typedef void * caddr_t;
#endif
#endif //_brk_h
