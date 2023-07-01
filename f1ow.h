#ifndef _F1OW_HH_
#define _F1OW_HH_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <netinet/ip.h> 
#include <netdb.h>
#define BUF_SIZE 65536

//freeipapi

// function prototype
int CreateRawSocket(char *interface_name);
void ReadRawPackets(int socket_desc);
void ParsingBinary(unsigned char *buffer, int buf_size);
void ExtractEthernet(unsigned char *buffer);
void ExtractIp(unsigned char *buffer);
void ExtractTCP(unsigned char *buffer);
void ExtractUDP(unsigned char *buffer);

void AllocateMemory();

// vars
unsigned char *buffer;
int bytes_read = 0;
int i, k, j;
struct ethhdr *ethernet_header;
struct iphdr *network_header;
struct in_addr saddr;
struct in_addr daddr;
struct protoent *proto;

#endif