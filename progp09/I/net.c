/*
net
a single threaded socket wrapper

Copyright (C) 2009 Evan Rinehart

This software comes with no warranty.           
1. This software can be used for any purpose, good or evil.             
2. Modifications must retain this license, at least in spirit.
*/

#ifdef WIN32

#define WINVER 0x0501
#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>



#include "net.h"

#define ERR_BUF_SIZE 256
#define ADDR_LEN 64
#define RECV_BUF_SIZE 512

struct address {
	struct sockaddr* val;
	socklen_t len;
};

struct net_socket {
	int fd;
	int connected;
	int datagram;
	int listening;
	address addr;

	void* ud;
	int (*recvcb)(net_socket*, byte[], int, void*);
	void (*connectcb)(net_socket* s, void* ud);
	byte buf[RECV_BUF_SIZE];
	int ptr;
	net_socket* next;
};





/*** global ***/

char error_buf[ERR_BUF_SIZE] = "";
char addr_buf[NI_MAXHOST];
address* fromaddr;
address* acceptaddr;
net_socket* callers;
int policy = 0;


/*** internal routines ***/



void insert_caller(net_socket* s){
	s->next = callers;
	callers = s;
}

void remove_caller(net_socket* s){
	net_socket* ptr = callers;
	net_socket* prev = NULL;

	if(callers == s){
		callers = s->next;
		return;
	}

	while(ptr != s){
		if(ptr == NULL) return;
		prev = ptr;
		ptr = ptr->next;
	}

	prev->next = ptr->next;
}

int maxfd(){
	int max = 0;
	net_socket* ptr = callers;
	while(ptr != NULL){
		if(ptr->fd > max) max = ptr->fd;
		ptr = ptr->next;
	}
	return max;
}


void* xmalloc(size_t count){
	void* m = malloc(count);
	if(!m){
		fprintf(stderr, "net: OUT OF MEMORY\n");
		exit(-2);
	}
	else{
		return m;
	}
}

void seterror(const char* errstr){
	strncpy(error_buf, errstr, ERR_BUF_SIZE);
	error_buf[ERR_BUF_SIZE-1] = '\0';
}

void platform_error(){
#ifdef WIN32
	LPTSTR str;
	int e = WSAGetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
	FORMAT_MESSAGE_FROM_SYSTEM|
	FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL, e, 0, (LPTSTR)&str, 0, NULL);

	seterror((char*)str);
	LocalFree(str);
#else
	char* str = strerror(errno);
	seterror(str);
#endif
}

void copyaddr(address* a1, address* a2){
	int len = a1->len;
	a2->val = xmalloc(len);
	a2->len = len;
	memcpy(a2->val, a1->val, len);
}

int cmpaddr(address* a1, address* a2){
	if(a1->len != a2->len) return a1->len - a2->len;
	return memcmp(&a1->val, &a2->val, a1->len);
}

net_socket* create_socket(char* name, int port, int type, int flags){
	char portstr[NI_MAXSERV];
	struct addrinfo* res;
	struct addrinfo hints;

	snprintf(portstr, NI_MAXSERV, "%d", port);

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = type;
	hints.ai_protocol = 0;
	hints.ai_flags = flags;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	int e = getaddrinfo(name, portstr, &hints, &res);
	if(e<0){
		seterror(gai_strerror(e));
		return NULL;
	}


	struct addrinfo* choice = NULL;
	struct addrinfo* ptr = res;
	if(policy == 0){ /* use first address */
		choice = res;
	}
	else if(policy == 4){ /* use first ipv4 address */
		while(ptr && ptr->ai_family != AF_INET){
			ptr = ptr->ai_next;
		}
		choice = ptr;
	}
	else if(policy == 6){ /* use first ipv6 address */
		while(ptr && ptr->ai_family != AF_INET6){
			ptr = ptr->ai_next;
		}
		choice = ptr;
	}

	if(choice == NULL){
		freeaddrinfo(res);
		seterror("address not found");
		return NULL;
	}
	
	net_socket* s = malloc(sizeof(net_socket));
	s->fd = socket(choice->ai_family, type, 0);
	if(s->fd == -1){
		freeaddrinfo(res);
		free(s);
		platform_error();
		return NULL;
	}

	int opt = 1;
	if(setsockopt(s->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt))<0){
		freeaddrinfo(res);
		platform_error();
		close(s->fd);
		free(s);
		return NULL;
	}

	address tmp = {choice->ai_addr, choice->ai_addrlen};
	copyaddr(&tmp, &s->addr);

	s->connected = 0;
	s->datagram = 0;
	s->listening = 0;
	s->ptr = 0;
	s->ud = NULL;
	s->recvcb = NULL;
	s->connectcb = NULL;
	s->next = NULL;
	
	freeaddrinfo(res);

	return s;
}

void destroy_socket(net_socket* s){
	close(s->fd);
	free(s->addr.val);
	free(s);
}


void pump_procserv(net_socket* s){
	net_socket* c = net_accept(s);
	if(!c) return;
	s->connectcb(c, s->ud);
}

void pump_proc(net_socket* s){
	int n = recv(s->fd, s->buf+s->ptr, RECV_BUF_SIZE-s->ptr, 0);
	s->ptr += n;
	int r = 0;
	if(n == 0){
		r = s->recvcb(s, s->buf, -1, s->ud);
		if(r < 0){
			remove_caller(s);
			net_close(s);
			return;
		}
	}

	int shift = 0;
	do{
		r = s->recvcb(s, s->buf+shift, s->ptr-shift, s->ud);	
		if(r < 0){
			remove_caller(s);
			net_close(s);
			return;
		}
		shift += r;
	} while(r != 0 && shift != s->ptr);

        if(shift == s->ptr){
		s->ptr = 0;
		return;
	}

	int i,j;
	for(i=shift,j=0; i<s->ptr; i++, j++){
		s->buf[j] = s->buf[i];
	}
	s->ptr -= shift;
}







/*** public functions ***/




/** misc **/

void net_init(){

	int i;

	callers = NULL;
	
#ifdef WIN32
	WSAStartup(2.2, NULL);
#endif
  
	fromaddr = xmalloc(sizeof(address));
	fromaddr->len = ADDR_LEN;
	fromaddr->val = xmalloc(ADDR_LEN);

	acceptaddr = xmalloc(sizeof(address));
	acceptaddr->len = ADDR_LEN;
	acceptaddr->val = xmalloc(ADDR_LEN);

}

void net_shutdown(){
#ifdef WIN32
	WSACleanup();
#endif

	net_socket* ptr = callers;
	net_socket* next;
	while(ptr){
		next = ptr->next;
		net_close(ptr);
		ptr = next;
	}

	net_freeaddr(fromaddr);
	net_freeaddr(acceptaddr);
}

void net_close(net_socket* s){
	remove_caller(s);
	destroy_socket(s);
}

char* net_geterror(){
	return error_buf;
}

void net_setpolicy(int ipv){
	switch(ipv){
	case 0:
	case 4:
	case 6:
		policy = ipv;
	}
}




/** tcp **/

net_socket* net_connect(char* hostname, int port){
	net_socket* s = create_socket(hostname, port, SOCK_STREAM, 0);
	if(!s){
		return NULL;
	}

	if(connect(s->fd, s->addr.val, s->addr.len) == -1){
		platform_error();
		destroy_socket(s);
		return NULL;
	}

	s->connected = 1;

	return s;
}

net_socket* net_listen(int port){
	net_socket* s = create_socket(NULL, port, SOCK_STREAM, AI_PASSIVE);
	if(!s){
		return NULL;
	}

	if(bind(s->fd, s->addr.val, s->addr.len) == -1){
		platform_error();
		destroy_socket(s);
		return NULL;
	}

	if(listen(s->fd, 5) == -1){
		platform_error();
		destroy_socket(s);
		return NULL;
	}

	s->listening = 1;

	return s;
}

net_socket* net_listencb(int port, void (*cb)(net_socket* s, void* ud), void* ud){
	net_socket* s = net_listen(port);
	if(s==NULL){
		platform_error();
		return NULL;
	}
	s->connectcb = cb;
	s->ud = ud;
	insert_caller(s);
	return s;
}


net_socket* net_accept(net_socket* server){
	acceptaddr->len = ADDR_LEN;
	int fd;
	fd = accept(server->fd, acceptaddr->val, &acceptaddr->len);
	if(fd == -1){
		platform_error();
		return NULL;
	}
	
	net_socket* s = xmalloc(sizeof(net_socket));
	copyaddr(acceptaddr, &s->addr);
	s->fd = fd;
	s->datagram = 0;
	s->connected = 1;
	s->listening = 0;
	s->ud = NULL;
	s->connectcb = NULL;
	s->recvcb = NULL;
	s->next = NULL;
	s->ptr = 0;
	return s;
}






/** i/o **/

int net_send(net_socket* s, void* data, int count){
	if(s->datagram && s->addr.val){
		return net_sendto(s, data, count, &s->addr);
	}
	else if(s->connected){
		int n = send(s->fd, data, count, 0);
		if(n == -1){
			platform_error();
		}
		return n;
	}
	else{
		seterror("invalid socket");
		return -1;
	}
}

int net_recv(net_socket* s, void* buf, int count){
	for(;;){
		int n = recvfrom(s->fd, buf, count, 0, fromaddr->val, &fromaddr->len);
		if(!s->addr.val && cmpaddr(fromaddr, &s->addr)) continue;
		if(n == -1){
			platform_error();
		}
		return n;
	}		
}


int net_recvfrom(net_socket* s, void* data, int count, address** a){
	int n = recvfrom(s->fd, data, count, 0, 
	                 fromaddr->val, &fromaddr->len);
	if(n == -1){
		platform_error();
		return -1;
	}
	if(a) *a = fromaddr;
	return n;
}

int net_sendto(net_socket* s, void* data, int count, address* a){
	int n = sendto(s->fd, data, count, 0, a->val, a->len);
	if(n == -1){
		platform_error();
		return -1;
	}
	return n;
}







/** udp **/


net_socket* net_udp_open(int port){
	net_socket* s = create_socket(NULL, port, SOCK_DGRAM, AI_PASSIVE);
	if(!s){
		return NULL;
	}

	int opt = 1;
        if(setsockopt(s->fd, SOL_SOCKET, SO_BROADCAST, (void*)&opt, sizeof(opt))<0){
		platform_error();
		destroy_socket(s);
		return NULL;
	}


	if(bind(s->fd, s->addr.val, s->addr.len) == -1){
		platform_error();
		destroy_socket(s);
		return NULL;
	}

	s->datagram = 1;
	
	return s;
}



void net_udp_bind(net_socket* s, address* addr){
	if(s->addr.val) net_freeaddr(&s->addr);
	if(addr) copyaddr(addr, &s->addr);
}






/** address **/

address* net_resolve(char* hostname, int port){
	struct addrinfo* res;
        char portstr[NI_MAXSERV];
        snprintf(portstr, NI_MAXSERV, "%d", port);
	int e = getaddrinfo(hostname, portstr, NULL, &res);
	if(e<0){
		seterror(gai_strerror(e));
		return NULL;
	}

	struct addrinfo* choice = NULL;
	struct addrinfo* ptr = res;
	if(policy == 0){ /* use first address */
		choice = res;
	}
	else if(policy == 4){ /* use first ipv4 address */
		while(ptr && ptr->ai_family != AF_INET){
			ptr = ptr->ai_next;
		}
		choice = ptr;
	}
	else if(policy == 6){ /* use first ipv6 address */
		while(ptr && ptr->ai_family != AF_INET6){
			ptr = ptr->ai_next;
		}
		choice = ptr;
	}

	if(choice == NULL){
	        freeaddrinfo(res);
		return NULL;
	}


	address* addr = xmalloc(sizeof(address));
	address tmp = {res->ai_addr, res->ai_addrlen};
	copyaddr(&tmp, addr);
	
	return addr;
}

address* net_copyaddr(address* addr){
	address* copy = xmalloc(sizeof(address));
	copy->len = addr->len;
	copy->val = xmalloc(addr->len);
	memcpy(copy->val, addr->val, addr->len);
	return copy;
}

void net_freeaddr(address* addr){
	free(addr->val);
	free(addr);
}


address* net_getsockaddr(net_socket* s){
	return &s->addr;
}

char* net_ipaddr(address* addr){
	int e = getnameinfo(addr->val, addr->len, addr_buf, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
	if(e < 0){
		seterror(gai_strerror(e));
		return NULL;
	}
	else{
		return addr_buf;
	}
}

int net_port(address* addr){
        char buf[NI_MAXSERV];
	int e = getnameinfo(addr->val, addr->len, NULL, 0, buf, NI_MAXSERV, NI_NUMERICSERV);
	if(e < 0){
		seterror(gai_strerror(e));
		return -1;
	}
	else{
		int p = atoi(buf);
		return p;
  	}
}







/** callbacks **/

int net_pump(int ms){
	int i;
	void* ud;
  	fd_set fds;
	struct timeval timeout;
	struct timeval* tptr;
	net_socket* ptr;
	net_socket* next;

	FD_ZERO(&fds);

	ptr = callers;
	while(ptr != NULL){
		FD_SET(ptr->fd, &fds);
		ptr = ptr->next;
	}

	int m = maxfd();

	tptr = NULL;
	if(ms >= 0){
		timeout.tv_sec = ms / 1000;
		timeout.tv_usec = (ms % 1000) * 1000;
		tptr = &timeout;
	}

	int n = select(m+1, &fds, NULL, NULL, tptr);
	if(n==-1){
		platform_error();
		return -1;
	}
	if(n == 0){
		 return 0;
	}

	ptr = callers;
	while(ptr != NULL){
		next = ptr->next;
		if(FD_ISSET(ptr->fd, &fds)){
			if(ptr->listening) pump_procserv(ptr);
			else pump_proc(ptr);
		}
		ptr = next;
	}

	return 0;
}

void net_setcallback(net_socket* s, int (*cb)(net_socket*, byte[], int, void*), void* ud){
	remove_caller(s);
	if(cb) insert_caller(s);
	s->recvcb = cb;
	s->ud = ud;
}






/** message packing **/

int net_pack(byte buf[], size_t bufsize, char* format, ...){
	size_t ptr = 0;
	size_t index = 0;

	size_t N;
	char* s;
	int i;

	va_list ap;
	va_start(ap, format);

	while(format[index]){
		switch(format[index]){
		case 'b':
			if(ptr+1 >= bufsize) return -2;
			buf[ptr] = va_arg(ap, int);
			ptr += 1;
			break;
		case 'w':
			if(ptr+2 >= bufsize) return -2;
			*(uint16_t*)(buf+ptr) = htons(va_arg(ap, int));
			ptr += 2;
			break;
		case 'd':
			if(ptr+4 >= bufsize) return -2;
			*(uint32_t*)(buf+ptr) = htonl(va_arg(ap, uint32_t));
			ptr += 4;
			break;
		case 's':
			s = va_arg(ap, char*);

			size_t N = strlen(s);
			if(N > 65535) N = 65535;
			if(ptr+N+2 >= bufsize) return -2;
			*(uint16_t*)(buf+ptr) = htons(N);
			ptr += 2;

			for(i=0; s[i]!='\0'; i++){
				buf[ptr+i] = s[i];
			}
			ptr += N;
			break;
      		default:
			printf("net_pack: invalid format\n");
			return -1;
		}
		index += 1;
	}

	va_end(ap);
	
	return ptr;
}



int net_unpack(byte buf[], size_t count, char* format, ...){
	size_t ptr = 0;
	size_t index = 0;

	va_list ap;
	va_start(ap, format);

	uint8_t* b;
	uint16_t* w;
	uint32_t* d;
	char* str;

	int N, L;

	while(format[index]){
		if(ptr >= count) return 0; /* not enough data to unpack completely */

		switch(format[index]){
		case 'b':
			b = va_arg(ap, uint8_t*);
			*b = buf[ptr];
			ptr += 1;
			break;
		case 'w':
			w = va_arg(ap, uint16_t*);
			*w = ntohs(*(uint16_t*)(buf+ptr));
			ptr += 2;
			break;
		case 'd':
			d = va_arg(ap, uint32_t*);
			*d = ntohl(*(uint32_t*)(buf+ptr));
			ptr += 4;
			break;
		case 's':
			str = va_arg(ap, char*);
			N = ntohs(*(uint16_t*)(buf+ptr));
			ptr += 2;

			index += 1;
			if(format[index] != 'l'){
				printf("net_unpack: invalid format. s must be followed by l\n");
				return -1;
			}
			L = va_arg(ap, size_t);

			if(N < L){
				memcpy(str, buf+ptr, N);
				str[N] = '\0';
				ptr += N;
			}
			else{
				if(ptr + N >= count) return -2; /* bad protocol */
				memcpy(str, buf+ptr, L);
				str[L-1] = '\0';
				ptr += N;
			}
			break;
		case 'l':
			printf("unpack: format code l must follow code s\n");
			return -1;
		default:
			printf("unpack: invalid format\n");
			return -1;
		}
		index += 1;
	}
  
	return ptr;
}

