#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int getPacket(FILE * f,unsigned char *buf)
{
    int i,j,c;
    c = 0;
    char tmp[1024] = {0};
    fgets(tmp,1024,f);
    j = strlen(tmp);
    printf("packet:%s",tmp);
    buf[0] = 0x7e;
    c++;
    int end = 0;
    int vaild = 0;
    unsigned char x,y;
    unsigned char sum = 0;
    for(i=0;i<j;i++)
    {
        switch(tmp[i]){
            default:
                end = 1;
            case ' ':
                if(vaild)
                {
                    switch(x){
                        case 0x7e:
                            buf[c] = 0x7f;
                            c++;
                            buf[c] = 0x80;
                            c++;
                            break;
                        case 0x7f:
                            buf[c] = 0x7f;
                            c++;
                            buf[c] = 0x81;
                            c++;
                            break;
                        default:
                            buf[c] = x;
                            c++;
                    }
                }
                sum += x;
                x = 0;
                y = 0;
                vaild = 0;
                break;
            case '0':
                y = 0;
                vaild = 1;
                break;
            case '1':
                y = 1;
                vaild = 1;
                break;
            case '2':
                y = 2;
                vaild = 1;
                break;
            case '3':
                y = 3;
                vaild = 1;
                break;
            case '4':
                y = 4;
                vaild = 1;
                break;
            case '5':
                y = 5;
                vaild = 1;
                break;
            case '6':
                y = 6;
                vaild = 1;
                break;
            case '7':
                y = 7;
                vaild = 1;
                break;
            case '8':
                y = 8;
                vaild = 1;
                break;
            case '9':
                y = 9;
                vaild = 1;
                break;
            case 'a':
            case 'A':
                y = 10;
                vaild = 1;
                break;
            case 'b':
            case 'B':
                y = 11;
                vaild = 1;
                break;
            case 'c':
            case 'C':
                y = 12;
                vaild = 1;
                break;
            case 'd':
            case 'D':
                y = 13;
                vaild = 1;
                break;
            case 'e':
            case 'E':
                y = 14;
                vaild = 1;
                break;
            case 'f':
            case 'F':
                y = 15;
                vaild = 1;
                break;
        }
        if(end)
            break;
        x = x*16+y;
    }
    sum = 0x55-sum;
    switch(sum){
        case 0x7e:
            buf[c] = 0x7f;
            c++;
            buf[c] = 0x80;
            c++;
            break;
        case 0x7f:
            buf[c] = 0x7f;
            c++;
            buf[c] = 0x81;
            c++;
            break;
        default:
            buf[c] = sum;
            c++;
            break;
    }
    buf[c] = 0x7e;
    c++;
    if(c<4)
        c=0;
    return c;
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s ip port", argv[0]);
        exit(1);
    }
    printf("This is a UDP client\n");
    struct sockaddr_in addr;
    int sock;

    if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket");
        exit(1);
    }
    const int opt = 1;  
    //设置该套接字为广播类型，  
    int nb = 0;  
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
    if(nb == -1)  
    {  
        perror("SO_BROADCAST");
        exit(1);
    }  
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }
    unsigned char buff1[1024] = {0};
    FILE *file;
    file = fopen("packet.txt","r");
    if(file==NULL)
    {
        printf("can't open packet.txt");
    }
    while(1)
    {
        int n,i;
        n = getPacket(file,buff1);
        if(n<=0)
        {
            printf("data had been sended complete!\n");
            break;
        }
        n = sendto(sock, buff1,n, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0)
        {
            perror("\n sendto \n");
            close(sock);
            break;
        }
        printf("send successful\n");
        usleep(50000);
    }
    fclose(file);
    close(sock);
    return 0;
}

