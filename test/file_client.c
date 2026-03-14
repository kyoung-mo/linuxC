#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char file[100];
    int file_len;
    int n, in, out;
    if(argc!=4)
    {
		printf("Usage : %s <IP> <port> <file>\n", argv[0]);
		exit(1);
	} 
    if((in=open(argv[3], O_RDONLY) < 0))    // fd 생성
    {
        perror(argv[3]);
        return -1;
    }
	sock=socket(PF_INET, SOCK_STREAM, 0); // fd 생성
    if(sock < 0)
		error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
		error_handling("connect() error!");
    file_len=read(in, file, sizeof(file)-1); // in으로 교체
    if(file_len < 0)
        error_handling("read() error!");

    /* 원래 client.c 코드 -> 파일 전송 코드로 교체
    printf("Message from server: %s \n", message);
    close(sock);
    return 0;
    */

    do{
        n=read(in,file,sizeof(file));   // argv[3]에 해당하는 fd내용 읽음
        if(n>0)
        {
            write(sock,file,n);     // sock에 읽어온 데이터 쓰기
            printf("size : %d\b",n);
        }
        else if(n==0)
        {
            fputs("Done..",stderr);
            fputc('\n',stderr);
            break;
        }
        else
            break;
    }while(1);

	close(sock);
    close(in);
    return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
