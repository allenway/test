#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
void printPacket(unsigned char *buf,int size)
{
    if(buf[0]!=0x7e||buf[size-1]!=0x7e)
    {
        printf("packet is not correct 1\n");
    }
    int i,j;
    int err = 0;
    unsigned char sum = 0;
    printf("raw:");
    for(i=0,j=1;j<size-1;j++)
    {
        printf("%x ",buf[j]);
    }
    printf("\n");
    for(i=0,j=1;j<size-1;j++)
    {
        switch(buf[j]){
            case 0x7f:
                j++;
                if(j>=size)
                    err = 1;
                else
                {
                    if(buf[j]==0x80)
                        buf[i] = 0x7e;
                    else if(buf[j]==0x81)
                        buf[i] = 0x7f;
                    else
                        err = 1;
                }
                break;
            default:
                buf[i] = buf[j];
        }
        sum += buf[i];
        i++;
        if(err)
            break;
    }
    if(err)
        printf("packet is not correct 2\n");
    else
    {
        if(sum!=0x55)
          printf("packet checksum is not correct,checksum:%x\n",sum);
        else
        {
            int j = 0;
            printf("packet:");
            for(j=0;j<i-1;j++)
            {
                printf("%x  ",buf[j]);
            }
            printf("\n");
        }
    }
}
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s port\n", argv[0]);
        exit(1);
    }
    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    unsigned char buff[1024];
    struct sockaddr_in clientAddr;
    int n;
    int len = sizeof(clientAddr);
    while (1)
    {
        n = recvfrom(sock, buff,1024, 0, (struct sockaddr*)&clientAddr, &len);
        if (n>0)
        {
            buff[n] = 0;
            printf("packet from %s %u\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            printPacket(buff,n);
        }
        else
        {
            perror("recv");
            break;
        }
    }
    return 0;
}

