#include "f1ow.h"

int main() {
    int s = CreateRawSocket("wlo1");
    AllocateMemory();
    ReadRawPackets(s);
}

void ExtractIp(unsigned char* buffer) {
    network_header = (struct iphdr*)(buffer+sizeof(struct ethhdr));
    saddr.s_addr = network_header->saddr;
    daddr.s_addr = network_header->daddr;

    printf("..:: IP Header ::..\n");
    printf("Total length of packet: %hu\n", network_header->tot_len);
    proto = getprotobynumber(network_header->protocol);
    if(proto != NULL)
        printf("\033[0;32mProtocol: %s\033[0m\n", proto->p_name);   
    printf("\033[0;32mSource IP: \t\t\t%s\033[0m\n", inet_ntoa(saddr));
    printf("\033[0;32mDestination IP: \t\t%s\033[0m\n\n", inet_ntoa(daddr));
}

void ExtractEthernet(unsigned char *buffer) {
    ethernet_header = (struct ethhdr*)buffer;

    printf("\n..:: Ethernet Header ::..\n");
    printf("Source Mac: \t\t\t%02X-%02X-%02X-%02X-%02X-%02X\nDestination Mac: \t\t%02X-%02X-%02X-%02X-%02X-%02X\n\n", ethernet_header->h_source[0], ethernet_header->h_source[1], ethernet_header->h_source[2], ethernet_header->h_source[3], ethernet_header->h_source[4], ethernet_header->h_source[5], ethernet_header->h_dest[0],ethernet_header->h_dest[1],ethernet_header->h_dest[2],ethernet_header->h_dest[3],ethernet_header->h_dest[4],ethernet_header->h_dest[5]);
}

void AllocateMemory() {
    ethernet_header = (struct ethhdr*)malloc(sizeof(struct ethhdr));
    network_header = (struct iphdr*)malloc(sizeof(struct iphdr));
}
void ParsingBinary(unsigned char *buffer, int buf_size) {
    for(i = 0; i < buf_size; i += 17) {
        printf("%08X: ", i);
        for(j = 0; j < 16; j++) {
            printf("%02X ", buffer[i+j]);
        }

        printf(" | ");

        for(k = 0; k < 16; k++) {
            if(isprint(buffer[i+k])) {
                printf("%c", buffer[i+k]);
            } else {
                printf(".");
            }
        }
        printf("|   \n");
    }
}

void ReadRawPackets(int socket_desc) {
    buffer = (unsigned char*)malloc(BUF_SIZE);
    while(1) {
        bytes_read = recvfrom(socket_desc, buffer, BUF_SIZE, 0, NULL, NULL);
        ParsingBinary(buffer, bytes_read);
        ExtractEthernet(buffer);
        ExtractIp(buffer);
    }
}

int CreateRawSocket(char *interface_name) {
    int sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sock_r == -1) {
        perror("socket: ");
        return sock_r;
    }

    int setopt = setsockopt(sock_r, SOL_SOCKET, SO_BINDTODEVICE, interface_name, strlen(interface_name));

    if(setopt == -1) {
        perror("setsockopt: ");
        return -1;
    }

    return sock_r;
}

