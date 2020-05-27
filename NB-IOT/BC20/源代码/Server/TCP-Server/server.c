#include<errno.h>
#include<sys/socket.h>
#include<resolv.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	int i = 0;
    int server_sockfd,client_sockfd;
    unsigned int myport = 8388;
    socklen_t len;
    struct sockaddr_in server_addr,client_addr;
    char buf[1024];
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(myport);

	server_addr.sin_addr.s_addr=INADDR_ANY;

    if((server_sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
		printf("create socket failed\n");
        return 1;
    }

    if(bind(server_sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
		printf("bind failed\n");
        return 2;
    }

    if(listen(server_sockfd,5) < 0)
    {
    	printf("listen failed\n");
        return 3;
    }
	
	len = sizeof(struct sockaddr);
	printf("Server IP is %s Port is %d\n","INADDR_ANY",myport);
	printf("Waiting client connect.....\n");
	client_sockfd=accept(server_sockfd,(struct sockaddr*)&client_addr,(socklen_t*)&len);
	
	if(client_sockfd < 0)
	{
		perror("accept failed\n");
		return 4;
	}
	else
	{
		printf("connected with IP:%s and port:%d\n",inet_ntop(AF_INET,&client_addr.sin_addr,buf,1024),ntohs(client_addr.sin_port));
	}
	
	printf("Waiting client data\n");

	while(1)
	{
		memset(buf,0,sizeof(buf));
		ssize_t size = read(client_sockfd,buf,sizeof(buf)-1);
		
		if(size > 0)
		{
			printf("client#:%s\n",buf);
		}
		else if(size == 0)
		{
			printf("read is done...\n");
		}
		else
		{
			perror("read failed\n");
			break;
		}
		
		for(i = 0; i < 8; i++)
		{
			buf[i] = 'a' + i;
		}
		buf[i] = '\0';
		write(client_sockfd,buf,strlen(buf));
	}
	
	close(server_sockfd);
	return 0;
}
