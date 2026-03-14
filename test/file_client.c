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
    int n, in;
    if(argc!=4)
    {
		printf("Usage : %s <IP> <port> <file>\n", argv[0]);
		exit(1);
	}
    
    // 복사의 원본이 되는 파일 읽기 모드로 열기
    // 복사 원본이면 클라이언트 argv[3]
    if((in=open(argv[3], O_RDONLY, S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH)) < 0)
    {
        perror(argv[3]);
        return -1;
    }

	sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0)
		error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
		error_handling("connect() error!");
    file_len=read(sock, file, sizeof(file)-1);
    if(file_len<0)
        error_handling("read() error!");
    do{
        n=read(in,file,sizeof(file));
        if(n>0)
        {
            write(sock,file,n);
        }
        else if(n==0)
        {
            break;
        }
        else
            break;
    }while(1);

	
	
	close(sock);
	
    return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
