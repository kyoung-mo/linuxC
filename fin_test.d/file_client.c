#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *file);
int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char file[100];
    int n;
    int in;
    if(argc!=4)
    {
		printf("Usage : %s <IP> <port> <file>\n", argv[0]);
		exit(1);
	}
    if((in=open(argv[3], O_RDONLY)) < 0)    // fd 생성
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

    do{
        // printf("in=%d, sock=%d\n",in,sock); // fd 확인
        n=read(in,file,sizeof(file)); 
        if(n>0)
            write(sock, file, n); 
        else if(n==0)
        {
            fputs("Done..",stderr);
            fputc('\n',stderr);
            break;
        }
        else
        {
            perror("read():");
            breakl
        }

    }while(1);

	close(sock);
    close(in);
    return 0;
}
void error_handling(char *file)
{
	fputs(file, stderr);
	fputc('\n', stderr);
	exit(1);
}
