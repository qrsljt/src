#ifndef __TOU_H_
#define __TOU_H_

#include<time.h>

#define BUFFER_SIZE 128
#define MAXSIZE 128
#define USERBUF_SIZE 32
#define PORT 8888


typedef struct aaa{
	int aaa;
	int permission;
	char operation[32];
	char user[USERBUF_SIZE];
	char password[USERBUF_SIZE];
	char buffer[BUFFER_SIZE];
	char addr[128]; // 地址
	char age[32]; // 年龄
	char level[32]; // 等级
	char no[32]; //编号
	char phone[32]; // 电话
	char salary[32] ; // 工资
}employee_info;  

void yhdl();
void ptyhts();
void glyjmts();
void fwqtjcjyh();
void glyxgyhxx(int i,employee_info info);
void glyjm(int i,employee_info info);
void glyscyh(int i,employee_info info);
void ptyhdl(int i,employee_info info);
void ptyhxgmm(int i,employee_info info);
void ptyhjm(int i,employee_info info);
void ptyhcx(int i,employee_info info);
void glydl(int i,employee_info info);
void glyyhcx(int i,employee_info info);
void glycxqb(int i,employee_info info);
void glytjyh(int i,employee_info info);
void glycxlsjl(int i,employee_info info);
void fwqdlpd(int i,employee_info info,sqlite3 *db);
void fwqptyhcx(int i,employee_info info,sqlite3 *db);
void fwqglydlpd(int i,employee_info info,sqlite3 *db);
void fwqptyhxgmm(int i,employee_info info,sqlite3 *db);
void fwqglyjm(int i,employee_info info,sqlite3 *db);
void fwqglycxqb(int i,employee_info info,sqlite3 *db);
void fwqglytjyh(int i,employee_info info,sqlite3 *db);
void fwqglyscyh(int i,employee_info info,sqlite3 *db);
void fwqglyxgyhxx(int i,employee_info info,sqlite3 *db);
void fwqcx_history(int i,employee_info info,sqlite3 *db);
void fwq_history(int i,employee_info info,sqlite3 *db);

void yhdl(){
	printf("\n");
	printf("\n");
	printf("**************************************************\n");
	printf("********1.普通用户    2.管理员     3.退出*********\n");
	printf("**************************************************\n");
	printf("input > ");
}

//普通用户登录
void ptyhdl(int i,employee_info info){
	int n;
	info.permission = 0;
	printf("请输入用户名>>>");
	fgets(info.user,sizeof(info.user),stdin);
	printf("请输入密码>>>");
	fgets(info.password,sizeof(info.password),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉
	info.password[strlen(info.password) - 1] = '\0'; //将'\n'去掉

	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}
	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("登录成功!\n");
			ptyhjm(i,info);
		}else{
			printf("登录失败!\n");
			memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
		}
	}
	
}
//服务器登录查询账户密码是否正确
void fwqdlpd(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	char **result;
	int nrow,ncolum,index,k,j;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from usertable where user='%s' and password='%s';",info.user,info.password);
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}

	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(nrow){
		strcpy(info.buffer,"y");
		send(i, &info, sizeof(info),0);
		fwq_history(i,info,db);
	}else{
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}

	sqlite3_free_table(result);
}

void ptyhts(){
	printf("\n");
	printf("\n");
	printf("**************************************************\n");
	printf("*****1.查询个人信息     2.修改密码     3.退出*****\n");
	printf("**************************************************\n");
	printf("input > ");
}

void ptyhjm(int i,employee_info info){
	char buf[32];	
	memset(buf,0,sizeof(buf));
	while(1){
		ptyhts();
		fgets(buf,sizeof(buf),stdin);
		if(strcmp(buf,"1\n") == 0){
			ptyhcx(i,info);
		}else if(strcmp(buf,"2\n") == 0){
			ptyhxgmm(i,info);
		}else if(strcmp(buf,"3\n") == 0){
			break;
		}else{
			printf("请按要求输入!\n");
		}
	}
}

void ptyhcx(int i,employee_info info){
	int n;
	info.aaa = 1;
	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}

	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("用户           地址           年龄           等级           工号           电话           工资\n");
			printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",info.user,info.addr,info.age,info.level,info.no,info.phone,info.salary);
		}else{
			printf("您的信息暂时未录入系统!\n");
		}
	}
	
}

void ptyhxgmm(int i,employee_info info){
	int n;
	info.aaa = 2;	
	printf("请输新密码>>>");
	fgets(info.password,sizeof(info.password),stdin);
	info.password[strlen(info.password) - 1] = '\0'; //将'\n'去掉
	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}

	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("修改成功！\n");
		}else{
			printf("修改失败!\n");
		}
	}
}

void fwqptyhcx(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	char **result;
	int nrow,ncolum,index,k,j,n;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from info where user='%s';",info.user);
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}
	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(nrow){
		strcpy(info.buffer,"y");
		strcpy(info.user,result[7]);
		strcpy(info.addr,result[8]);
		strcpy(info.age,result[9]);
		strcpy(info.level,result[10]);
		strcpy(info.no,result[11]);
		strcpy(info.phone,result[12]);
		strcpy(info.salary,result[13]);
		printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",info.user,info.addr,info.age,info.level,info.no,info.phone,info.salary);
		send(i, &info, sizeof(info),0);
		fwq_history(i,info,db);
	}else{
		memset(info.buffer,0,sizeof(info.buffer));
		strcpy(info.buffer,"n\0");
		send(i, &info, sizeof(info),0);
	}
	sqlite3_free_table(result);

}

void fwqptyhxgmm(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"update usertable set password='%s' where user='%s';",info.password,info.user);
	printf("%s\n",buf);	
	if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
		printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
		fwq_history(i,info,db);
	}else{
		strcpy(info.buffer,"y");
		send(i, &info, sizeof(info),0);
	}
}

/******************************管理员*********************************************/

void glydl(int i,employee_info info){
	int n;
	info.aaa = 3;
	info.permission = 1;
	printf("请输入用户名>>>");
	fgets(info.user,sizeof(info.user),stdin);
	printf("请输入密码>>>");
	fgets(info.password,sizeof(info.password),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉
	info.password[strlen(info.password) - 1] = '\0'; //将'\n'去掉

	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}
	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("登录成功!\n");
			glyjm(i,info);
		}else{
			printf("登录失败!\n");
		}
	}
	
}

void fwqglydlpd(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	char **result;
	int nrow,ncolum,index,k,j;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from usertable where user='%s' and password='%s' and permission ='1';",info.user,info.password);
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}

	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(ncolum){
		strcpy(info.buffer,"y");
		send(i, &info, sizeof(info),0);
		fwq_history(i,info,db);
	}else{
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}

	sqlite3_free_table(result);
	
}

void glyjmts(){
	printf("\n");
	printf("\n");
	printf("********************************************************************************\n");
	printf("*1.按用户查询 2.查询全部 3.添加用户 4.删除用户 5.修改用户信息 6.历史记录 7.退出*\n");
	printf("********************************************************************************\n");
	printf("input > ");
}


void glyjm(int i,employee_info info){
	char buf[32];	
	memset(buf,0,sizeof(buf));
	while(1){
		glyjmts();
		fgets(buf,sizeof(buf),stdin);
		if(strcmp(buf,"1\n") == 0){
			glyyhcx(i,info);
		}else if(strcmp(buf,"2\n") == 0){
			glycxqb(i,info);
		}else if(strcmp(buf,"3\n") == 0){
			glytjyh(i,info);
		}else if(strcmp(buf,"4\n") == 0){
			glyscyh(i,info);
		}else if(strcmp(buf,"5\n") == 0){
			glyxgyhxx(i,info);
		}else if(strcmp(buf,"6\n") == 0){
			glycxlsjl(i,info);
		}else if(strcmp(buf,"7\n") == 0){
			break;
		}else{
			printf("请按要求输入!\n");
		}
	}
}




void fwqglyyhcx(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	char **result;
	int nrow,ncolum,index,k,j;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from info where user='%s';",info.user);
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}
	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(ncolum){
		memset(&info,0,sizeof(info));
		strcpy(info.buffer,"y");
		strcpy(info.user,result[7]);
		strcpy(info.addr,result[8]);
		strcpy(info.age,result[9]);
		strcpy(info.level,result[10]);
		strcpy(info.no,result[11]);
		strcpy(info.phone,result[12]);
		strcpy(info.salary,result[13]);
		send(i, &info, sizeof(info),0);
		fwq_history(i,info,db);
	}else{
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}
	
	sqlite3_free_table(result);
}

void glyyhcx(int i,employee_info info){
	int n;
	info.aaa = 4;
	printf("请输入要查询的用户名>>>");
	fgets(info.user,sizeof(info.user),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉
	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}

	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("用户           地址           年龄           等级           工号           电话           工资\n");
			printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",info.user,info.addr,info.age,info.level,info.no,info.phone,info.salary);
		}else{
			printf("您查询的信息暂时未录入系统!\n");
		}
	}
	
}

void glycxqb(int i,employee_info info){
	int n;
	info.aaa = 5;
	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}

	while(1){
		memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
		if((n = recv(i,&info,sizeof(info),0)) == -1){
			perror("recv error");
			exit(-1);
		}else{
			if(strcmp(info.buffer,"y\0") == 0){
				printf("用户           地址           年龄           等级           工号           电话           工资\n");
				printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n",info.user,info.addr,info.age,info.level,info.no,info.phone,info.salary);
			}else if(strcmp(info.buffer,"ok!\0") == 0){
				break;
			}else{
				printf("您的信息暂时未录入系统!\n");
				break;
			}
		}
	}

}

void fwqglycxqb(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	char **result;
	int nrow,ncolum,index,k,j;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from info;");
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}
	index = 7;
	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(nrow){
		for(k = 0; k < nrow; k++){
			memset(&info,0,sizeof(info));
			strcpy(info.buffer,"y");
			strcpy(info.user,result[index++]);
			strcpy(info.addr,result[index++]);
			strcpy(info.age,result[index++]);
			strcpy(info.level,result[index++]);
			strcpy(info.no,result[index++]);
			strcpy(info.phone,result[index++]);
			strcpy(info.salary,result[index++]);
			send(i, &info, sizeof(info),0);
			fwq_history(i,info,db);
		}
		strcpy(info.buffer,"ok!");
		send(i, &info, sizeof(info),0);
	}else{
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}
	
	sqlite3_free_table(result);
}
/******************************添加用户********************************************/

void glytjyh(int i,employee_info info){
	int n;
	info.aaa = 6;
	info.permission = 0;
	printf("请输入用户名>>>");
	fgets(info.user,sizeof(info.user),stdin);
	printf("请输入密码>>>");
	fgets(info.password,sizeof(info.password),stdin);
	printf("请输入地址>>>");
	fgets(info.addr,sizeof(info.addr),stdin);
	printf("请输入年龄>>>");
	fgets(info.age,sizeof(info.age),stdin);
	printf("请输入等级>>>");
	fgets(info.level,sizeof(info.level),stdin);
	printf("请输入工号>>>");
	fgets(info.no,sizeof(info.no),stdin);
	printf("请输入电话>>>");
	fgets(info.phone,sizeof(info.phone),stdin);
	printf("请输入工资>>>");
	fgets(info.salary,sizeof(info.salary),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉
	info.password[strlen(info.password) - 1] = '\0'; //将'\n'去掉
	info.addr[strlen(info.addr) - 1] = '\0'; //将'\n'去掉
	info.age[strlen(info.age) - 1] = '\0'; //将'\n'去掉
	info.level[strlen(info.level) - 1] = '\0'; //将'\n'去掉
	info.no[strlen(info.no) - 1] = '\0'; //将'\n'去掉
	info.phone[strlen(info.phone) - 1] = '\0'; //将'\n'去掉
	info.salary[strlen(info.salary) - 1] = '\0'; //将'\n'去掉

	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}
	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("添加用户成功!\n");
		}else{
			printf("添加用户失败!\n");
		}
	}
}

void fwqglytjyh(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"insert into info values('%s','%s','%s','%s','%s','%s','%s');",info.user,info.addr,info.age,info.level,info.no,info.phone,info.salary);
	printf("%s\n",buf);	
	if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
		printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}else{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"insert into usertable values(0,'%s','%s');",info.user,info.password);
		printf("%s\n",buf);	
		if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
			printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);
			strcpy(info.buffer,"n");
			send(i, &info, sizeof(info),0);
		}else{
			strcpy(info.buffer,"y");
			send(i, &info, sizeof(info),0);
			fwq_history(i,info,db);
		}
	}
}

/**************************************管理员删除用户*********************************************/
void glyscyh(int i,employee_info info){
	int n;
	info.aaa = 7;
	printf("请输入要删除的用户名>>>");
	fgets(info.user,sizeof(info.user),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉

	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}
	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("删除用户成功!\n");
		}else{
			printf("删除用户失败!\n");
		}
	}	
}


void fwqglyscyh(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"delete from info where user='%s';",info.user);
	printf("%s\n",buf);	 
	if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
		printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}else{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"delete from usertable where user='%s';",info.user);
		printf("%s\n",buf);	
		if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
			printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);
			strcpy(info.buffer,"n");
			send(i, &info, sizeof(info),0);
		}else{
			strcpy(info.buffer,"y");
			send(i, &info, sizeof(info),0);
			fwq_history(i,info,db);
		}
	}
	
}
/***********************************管理员修改信息*******************************************/
void glyxgyhxx(int i,employee_info info){
	int n;
	info.aaa = 8;	
	printf("请输要修改的用户名(因为太鸡儿麻烦，在此只修改工资)>>>");
	fgets(info.user,sizeof(info.user),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉
	printf("请输工资>>>");
	fgets(info.salary,sizeof(info.salary),stdin);
	info.salary[strlen(info.salary) - 1] = '\0'; //将'\n'去掉
	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}

	memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
	if((n = recv(i,&info,sizeof(info),0)) == -1){
		perror("recv error");
		exit(-1);
	}else{
		if(strcmp(info.buffer,"y\0") == 0){
			printf("修改成功！\n");
		}else{
			printf("修改失败!\n");
		}
	}
		
}

void fwqglyxgyhxx(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;


	char **result;
	int nrow,ncolum,index,k,j;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from info where user='%s';",info.user);
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}
	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(nrow){
		memset(buf,0,sizeof(buf));
		sprintf(buf,"update info set salary='%s' where user='%s';",info.salary,info.user);
		printf("%s\n",buf);	
		if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
			printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);
			strcpy(info.buffer,"n");
			send(i, &info, sizeof(info),0);
		}else{
			strcpy(info.buffer,"y");
			send(i, &info, sizeof(info),0);
			fwq_history(i,info,db);
		}
	}else{
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}

}

/************************************* 历史记录******************************************/
void fwq_history(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	time_t timep;
	struct tm *p;
	time (&timep);
	p=gmtime(&timep);
	memset(buf,0,sizeof(buf));
	switch(info.aaa){
	case 0:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','登录');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 1:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','查询信息');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 2:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','修改密码');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 3:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','管理员登录');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 4:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','管理员查询信息');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 5:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','管理员查询全部信息');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 6:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','添加用户');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 7:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','删除用户');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
	case 8:
		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','修改用户信息');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
		break;
//	case 9:
//		sprintf(buf,"insert into history values('%s','%d年%d月%d日%d时%d分%d秒','');",info.user,1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
//		break;
	}
	printf("%s\n",buf);	
	if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
		printf("sqlite3_exec update fail!!!errmsg = %s\n",errmsg);

//		strcpy(info.buffer,"n");
//		send(i, &info, sizeof(info),0);
	}else{
//		strcpy(info.buffer,"y");
//		send(i, &info, sizeof(info),0);
		printf("添加历史记录成功!\n");
	}


}

void glycxlsjl(int i,employee_info info){
	int n;
	info.aaa = 9;
	if(send(i,&info,sizeof(info),0) == -1){
		perror("send error");
		exit(-1);
	}

	while(1){
		memset(info.buffer,0,sizeof(info.buffer)); //清空buffer
		if((n = recv(i,&info,sizeof(info),0)) == -1){
			perror("recv error");
			exit(-1);
		}else{
			if(strcmp(info.buffer,"n\0") == 0){
				printf("暂无历史记录!\n");
				break;
			}else if(strcmp(info.buffer,"ok!\0") == 0){
				break;
			}else{
				printf("%s\n",info.buffer);
			}
		}
	}
	
}

void fwqcx_history(int i,employee_info info,sqlite3 *db){
	char buf[128];
	char *errmsg;
	char **result;
	int nrow,ncolum,index,k,j,c = 0;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"select * from history;");
	printf("%s\n",buf);	
	if(sqlite3_get_table(db,buf,&result,&nrow,&ncolum,&errmsg) != 0){
		perror("sqlite3_get_table fail!!!!");
		return;
	}
	index = 3;
	printf("表格共%d 记录!\n", nrow);
	printf("表格共%d 列!\n", ncolum);
	if(nrow){
		for(k = 0; k < nrow; k++){
			memset(info.buffer,0,sizeof(info.buffer));
			sprintf(info.buffer,"%s,%s,%s",result[index],result[index+1],result[index+2]);
			index = index + 3;
			send(i, &info, sizeof(info),0);
		}
		strcpy(info.buffer,"ok!");
		send(i, &info, sizeof(info),0);
	}else{
		strcpy(info.buffer,"n");
		send(i, &info, sizeof(info),0);
	}
	
	sqlite3_free_table(result);
	
}

/*******************************************************************************/

void fwqtjcjyh(sqlite3 *db){
	int n;
	char buf[128];
	char *errmsg;
	employee_info info;
	memset(&info,0,sizeof(info));
	info.permission = 1;
	printf("请输入要创建的超级用户名>>>");
	fgets(info.user,sizeof(info.user),stdin);
	printf("请输入密码>>>");
	fgets(info.password,sizeof(info.password),stdin);
	printf("请输入地址>>>");
	fgets(info.addr,sizeof(info.addr),stdin);
	printf("请输入年龄>>>");
	fgets(info.age,sizeof(info.age),stdin);
	printf("请输入等级>>>");
	fgets(info.level,sizeof(info.level),stdin);
	printf("请输入工号>>>");
	fgets(info.no,sizeof(info.no),stdin);
	printf("请输入电话>>>");
	fgets(info.phone,sizeof(info.phone),stdin);
	printf("请输入工资>>>");
	fgets(info.salary,sizeof(info.salary),stdin);
	info.user[strlen(info.user) - 1] = '\0'; //将'\n'去掉
	info.password[strlen(info.password) - 1] = '\0'; //将'\n'去掉
	info.addr[strlen(info.addr) - 1] = '\0'; //将'\n'去掉
	info.age[strlen(info.age) - 1] = '\0'; //将'\n'去掉
	info.level[strlen(info.level) - 1] = '\0'; //将'\n'去掉
	info.no[strlen(info.no) - 1] = '\0'; //将'\n'去掉
	info.phone[strlen(info.phone) - 1] = '\0'; //将'\n'去掉
	info.salary[strlen(info.salary) - 1] = '\0'; //将'\n'去掉

	memset(buf,0,sizeof(buf));
	sprintf(buf,"insert into info values('%s','%s','%s','%s','%s','%s','%s');",info.user,info.addr,info.age,info.level,info.no,info.phone,info.salary);
	printf("%s\n",buf);	
	if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
		printf("sqlite3_exec insert fail!!!errmsg = %s\n",errmsg);
	}else{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"insert into usertable values(1,'%s','%s');",info.user,info.password);
		printf("%s\n",buf);	
		if(sqlite3_exec(db,buf,NULL,NULL,&errmsg)){
			printf("sqlite3_exec insert fail!!!errmsg = %s\n",errmsg);
			printf("创建失败!\n");
		}else{
			printf("创建成功!\n");
		}
	}
}

#endif
