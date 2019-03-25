#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include "tou.h"

int main()
{
	int i,nbyte;
	sqlite3 *db;
	char *errmsg;
	int listenfd, confd, maxfd;
	fd_set global_rdfs, current_rdfs;
	struct sockaddr_in addr,clientaddr;
	int addrlen = sizeof(struct sockaddr_in);
	int caddrlen = sizeof(struct sockaddr_in);
	employee_info info;
	memset(&info,0,sizeof(info));

	if(sqlite3_open("lib.db",&db)){
		perror("sqlite3_open fail!!!!");
		return -1;
	}
	printf("sqlite3_open successfully!\n");

	if(sqlite3_exec(db,"create table history(user char,time char,operation char);",NULL,NULL,&errmsg)){
		if(strcmp(errmsg,"table history already exists")){
			printf("sqlite3_exec history fail!!!errmsg = %s\n",errmsg);
			return -1;
		}
	}
	printf("sqlite3_exec create history table successfully!!!\n");

	if(sqlite3_exec(db,"create table info (user char primary key,addr char,age char,level char,no char,phone char,salary char);",NULL,NULL,&errmsg)){
		if(strcmp(errmsg,"table info already exists")){
			printf("sqlite3_exec info fail!!!errmsg = %s\n",errmsg);
			return -1;
		}
	}
	printf("sqlite3_exec create info table successfully!!!\n");

	if(sqlite3_exec(db,"create table usertable (permission int,user char primary key,password int);",NULL,NULL,&errmsg)){
		if(strcmp(errmsg,"table usertable already exists")){
			printf("sqlite3_exec user fail!!!errmsg = %s\n",errmsg);
			return -1;
		}
	}else{
		fwqtjcjyh(db);
	}
	printf("sqlite3_exec create user table successfully!!!\n");

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket error");
		exit(-1);
	}
	else{
		printf("socket successfully!\n");
		printf("listenfd : %d\n",listenfd);
	}

	memset(&addr, 0 ,addrlen);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listenfd,(struct sockaddr *)&addr,addrlen) == -1){
		perror("bind error");
		exit(-1);
	}
	else{
		printf("bind successfully!\n");
		printf("listen port:%d\n",PORT);
	}

	if(listen(listenfd,20) == -1){
		perror("listen error");
		exit(-1);
	}
	else{
		printf("listening...\n");
	}

	maxfd = listenfd;
	FD_ZERO(&global_rdfs);
	FD_SET(listenfd,&global_rdfs);

	while(1){
		current_rdfs = global_rdfs;
		if(select(maxfd + 1,&current_rdfs, NULL, NULL,0) < 0){
			perror("select error");
			exit(-1);
		}

		for(i = 0; i <= maxfd; i++){
			if(FD_ISSET(i, &current_rdfs)){
				if(i == listenfd){
					if((confd = accept(listenfd,(struct sockaddr *)&clientaddr,&caddrlen)) == -1){
						perror("accept error");
						exit(-1);
					}
					else{
						printf("Connect from [IP:%s PORT:%d]\n",inet_ntoa(clientaddr.sin_addr),clientaddr.sin_port);
						FD_SET(confd,&global_rdfs);
						maxfd = (maxfd > confd ? maxfd : confd);
					}
				}
				else{
					if((nbyte = recv(i, &info, sizeof(info),0)) < 0){
						perror("recv error");
						exit(-1);
					}
					else if(nbyte == 0){
						close(i);
						FD_CLR(i,&global_rdfs);
					}
					else{

						switch(info.aaa){
							case 0:
								fwqdlpd(i,info,db);
								break;
							case 1:
								fwqptyhcx(i,info,db);
								break;
							case 2:
								fwqptyhxgmm(i,info,db);
								break;
							case 3:
								fwqglydlpd(i,info,db);
								break;
							case 4:
								fwqglyyhcx(i,info,db);
								break;
							case 5:
								fwqglycxqb(i,info,db);
								break;
							case 6:
								fwqglytjyh(i,info,db);
								break;
							case 7:
								fwqglyscyh(i,info,db);
								break;
							case 8:
								fwqglyxgyhxx(i,info,db);
								break;
							case 9:
								fwqcx_history(i,info,db);
								break;
						}
					}
				}
			}
		}
	}

	return 0;
}
