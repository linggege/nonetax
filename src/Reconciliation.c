#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include "md5.h"


#define MYPORT  8887
#define BUFFER_SIZE 1024
//#define AREACODE 500102
//#define ACCOUNT 50001313600050004841
//20160303^|500102^|50001313600050004841^|ggfdffdfea^|
char areacode[6]="500102";
char account[20]="50001313600050004841";
char macstr[16];


void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        highByte += 0x30;

        if (highByte > 0x39)
            dest[i * 2] = highByte + 0x07;
        else
            dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}

int mac(char unsigned encrypt[])
{
    //char test1[16];
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
//    unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
    unsigned char decrypt[16];
    MD5Update(&md5,encrypt,strlen((char *)encrypt));
    MD5Final(&md5,decrypt);
//  打印mac
    printf("加密前:%s\n加密后:",encrypt);
    for(i=0; i<16; i++)
    {
        printf("%02x",decrypt[i]);
    }
//	打印mac

    ByteToHexStr(decrypt,macstr,16);
    printf("\nMAC：%s\n",macstr);
//    getchar();

    return 0;
}



int test()
{
    char b[]="20160303^|500102^|50001313600050004841^|3";
    printf("%lu\n",sizeof(b));
    return 0;
}

int main()
{
    //test();
    char sendbuf[128];
    char date[8];
    char *ptr;
    memset(sendbuf,0,sizeof(sendbuf));
    printf("Please input The Date:");
    fgets(date, sizeof(date), stdin);
    memset(sendbuf,0x00,sizeof(sendbuf));
    strcat(sendbuf,date);
    strcat(sendbuf,"^|");
    strcat(sendbuf,areacode);
    strcat(sendbuf,"^|");
    strcat(sendbuf,account);
    strcat(sendbuf,"^|");
    mac(sendbuf);

    strcat(sendbuf,macstr);
    strcat(sendbuf,"^|");
    //memcpy(sendbuf,"^|",sizeof(sendbuf));
    printf("send size:%lu\n",sizeof(sendbuf));
    printf("Send Massage:\n%s\n",sendbuf);
    /* code */
    // now send buf to server.
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    ///连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    //char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    //while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)

    send(sock_cli, sendbuf, strlen(sendbuf),0); ///发送
    // if(strcmp(sendbuf,"exit\n")==0)
    //     break;
    recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
    printf("Received Massage:\n");
    printf("%s\n",recvbuf);
    //fputs(recvbuf, stdout);
    //printf("re:%c\n",recvbuf[42]);
    close(sock_cli);
    // ptr = strtok(recvbuf, "^|");
    // while (ptr != NULL) {
    //   printf("ptr=%s\n", ptr);
    //     ptr = strtok(NULL, ",");
    // }
    return 0;
}
/*int main()
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    ///连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        send(sock_cli, sendbuf, strlen(sendbuf),0); ///发送
        if(strcmp(sendbuf,"exit\n")==0)
            break;
        recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
        fputs(recvbuf, stdout);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock_cli);
    return 0;
}
*/