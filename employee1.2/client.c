#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include "tou.h"
 
int main()
{
	int n;
	int serverfd, clientfd;
	struct sockaddr_in serveraddr;
	char buf[32];
	employee_info info;
	memset(&info,0,sizeof(info));
	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket error");
		exit(-1);
	}else{
		printf("clientfd:%d\n",clientfd);
	}
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(connect(clientfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
		perror("connect error");
		exit(-1);
	}else{
		printf("connect successfully!\n");
	}
	
	info.aaa = 0;
	while(1){	
		yhdl();
		memset(buf,0,sizeof(buf));
		fgets(buf,sizeof(buf),stdin);
		if(strcmp(buf,"1\n") == 0){
			ptyhdl(clientfd,info);
		}else if(strcmp(buf,"2\n") == 0){
			glydl(clientfd,info);
		}else if(strcmp(buf,"3\n") == 0){
			printf("大爷下次再来!\n");
			break;
		}else{
			printf("请按要求输入!\n");
		}
	}
	close(clientfd);

	return 0;
}
