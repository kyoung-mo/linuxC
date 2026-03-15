#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
void error_handling(char *file);
int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
    int out,n;  
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
    char file[100];
    if(argc!=3){
		printf("Usage : %s <port> <file>\n", argv[0]);
		exit(1);
	}
    if((out = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH))< 0) // fd 생성
    {
        perror(argv[2]);
        return -1;
    }
	serv_sock=socket(PF_INET, SOCK_STREAM, 0); // fd 생성
	if(serv_sock < 0)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr) )< 0 )
		error_handling("bind() error"); 
	if(listen(serv_sock, 5) < 0)
		error_handling("listen() error");
    clnt_addr_size=sizeof(clnt_addr);  
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
    //fd 생성

    if(clnt_sock < 0)
		error_handling("accept() error");
    
    do{
        n = read(clnt_sock, file, sizeof(file)); // 클라 fd 읽어서 n에 수신 바이트 저장
        if(n>0)
        {
            printf("read : %d\n",n);
            write(out, file, n);
        }
        else if(n==0)
        {
            fputs("Done..",stderr);
            fputc('\n',stderr);
            break;
        }
        else
        {
            write(n,file,n);
        }
    }while(1);
        
    close(clnt_sock);
	close(serv_sock);
    close(out);

	return 0;
}
void error_handling(char *file)
{
	fputs(file, stderr);
	fputc('\n', stderr);
	exit(1);
}
