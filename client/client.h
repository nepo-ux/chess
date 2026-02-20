#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#define PORT 2000

#define BUFFER_SIZE 1024
#define CHESS_MOVE_BUFFER_SIZE 6
#define DRAW_BUFFER 8


#define BRD_SQ_NUM 120

// Color constants
typedef enum { WHITE = 0, BLACK = 1 } Color;


typedef struct sockaddr_in SockAddrIn;
typedef struct sockaddr SockAddr;

#endif

