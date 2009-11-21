/*
net
a single threaded socket wrapper

Copyright (C) 2009 Evan Rinehart

This software comes with no warranty.
1. This software can be used for any purpose, good or evil.
2. Modifications must retain this license, at least in spirit.
*/

typedef struct net_socket net_socket;
typedef struct address address;
typedef unsigned char byte;

/* misc */
void net_init();
void net_shutdown();
char* net_geterror();
void net_close(net_socket* s);
void net_setpolicy(int ipv);

/* tcp */
net_socket* net_connect(char* hostname, int port);
net_socket* net_listen(int port);
net_socket* net_listencb(int port, void (*cb)(net_socket* s, void* ud), void* ud);
net_socket* net_accept(net_socket* serv);

/* udp */
net_socket* net_udp_open(int port);
void net_udp_bind(net_socket* s, address* remote);

/* i/o */
int net_send(net_socket* s, void* data, int count);
int net_recv(net_socket* s, void* buf, int count);
int net_sendto(net_socket* s, void* data, int count, address* addr);
int net_recvfrom(net_socket* s, void* buf, int count, address** addr);

/* callbacks */
int net_pump(int ms);
void net_setcallback(net_socket* s, int(*cb)(net_socket*, byte[], int, void*), void* ud);

/* addresses */
address* net_resolve(char* hostname, int port);
address* net_copyaddr(address* addr);
void net_freeaddr(address* addr);

address* net_getsockaddr(net_socket* s);
char* net_ipaddr(address* addr);
int net_port(address* addr);

/* message packing */
int net_pack(byte buf[], size_t bufsize, char* format, ...);
int net_unpack(byte buf[], size_t count, char* format, ...);
