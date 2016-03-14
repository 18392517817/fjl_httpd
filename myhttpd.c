/***********************************************************************
	> File Name: myhttpd.c
	> Author: fjl_57
	> Mail: 799619622@qq.com 
	> Created Time: Sun 06 Mar 2016 10:05:55 PM EST
************************************************************************/
#include"myhttpd.h"
#define SERVER_STRING "Server:jdbhttpd/1.0\r\n"
#define _COMM_SIZE_ 1024

void print_debug(const char * msg)
{
#ifdef _DEBUG_
	  printf("%s\n", msg);
#endif
}

void  clear_header(int sock_client)
{
    char buf[1024];
    memset(buf, '\0', sizeof(buf));
    int ret = 0;
    do{
        ret = get_line(sock_client, buf, sizeof(buf));
    }while(ret > 0 && strcmp(buf, "\n") != 0 );
	
}
//error message
void error_die(const char *sc)
{
	perror(sc);
}
//get the first line
int get_line(int sock,char *buf,int max_len)
{ 
		 
	if(!buf || (max_len<0)){
		return 0;
	}
	int i=0;
	int n=0;
	char c='\0';
	//
	while((i<max_len-1)&&(c!='\n'))	{
		n=recv(sock,&c,1,0);
		if(n>0){
			if(c=='\r'){
				n=recv(sock,&c,1,MSG_PEEK);
				if((n>0)&&(c=='\n')){
					recv(sock,&c,1,0);
				}else{
					c='\n';
				}
			}
			buf[i++]=c;
		}else{
			c='\n';
		}
		buf[i]='\0';
	}
	return i;
}

//bad_request
static void bad_request(int client)
{
	char buf[1024];
	
	sprintf(buf,"HTTP/1.0 400 BAD REQUEST\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"Countent-type: text/html\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<P>your enter message is bad request</p>\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"such as a POST without a Content-Length.\r\n");
	send(client,buf,strlen(buf),0);
	
}

static void cannot_execute(int client)
{
	char buf[1024];
	
	sprintf(buf,"HTTP/1.0 401 CANNOT EXECUTE\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"Countent-type: text/html\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<P>Cgi message is bad  </p>\r\n");
	send(client,buf,strlen(buf),0);
	 
	
}

//404
static void not_found(int client)
{
	
	char buf[1024];
	
	sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,SERVER_STRING);
	send(client,buf,sizeof(buf),0);
	sprintf(buf,"Countent-type: text/html\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<HTML><TITLE>NOT FOUND</TITLE>\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<BODY><P>The server could not fulfill\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"your request because the resource \r\n\
			specifild isunavailable or nonexistent.\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"</BODY></HTML>\r\n");
	send(client,buf,strlen(buf),0);
}


static void server_unavibale(int client)
{
	
	char buf[1024];
	
	sprintf(buf,"HTTP/1.0 501  METHOND NOT Implemented\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,SERVER_STRING);
	send(client,buf,strlen(buf),0);
	sprintf(buf,"Countent-Type: text/html\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<HTML><HEAD><TITLE>Method Not Implemented</TITLE></HEAD>\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"<BODY><P>HTTP request method not supported.\r\n");
	send(client,buf,strlen(buf),0);
	sprintf(buf,"</BODY></HTML>\r\n");
	send(client,buf,strlen(buf),0);
}

void echo_error_to_client(int client, int error_code)
{
	switch(error_code){
		case 400:
			bad_request(client);
			break;
		case 401:
			cannot_execute(client);
			break;
		case 404:
			not_found(client);
			break;
		case 501:
			server_unavibale(client);
			break;
		default:
		break;
		
	
	}

}
//GET index.html
void echo_html(int client,const char *path,unsigned int file_size)
{
	if(!path){
		return;
	}
	int in_fd=open(path,O_RDONLY);
	if(in_fd<0){
		print_debug("open index.html fail");
		echo_error_to_client(client,404);
		return ;
	}
	print_debug("open index.html success");
	char echo_line[1024];
	memset(echo_line,'\0',sizeof(echo_line));
	strncpy(echo_line,HTTP_VERSION,strlen(HTTP_VERSION)+1);
	strcat(echo_line, " 200 OK");
    strcat(echo_line, "\r\n\r\n");
    send(client, echo_line,strlen(echo_line), 0);
    print_debug("send echo head success");

	if(sendfile(client,in_fd,NULL,file_size)<0){
			print_debug("send_file fail");
		echo_error_to_client(client,404);//
		close(in_fd);
		return ;
	}
	print_debug("sendfile success");
	close(in_fd);
}

//exe_cgi
void exe_cgi(int sock_client,const char *path,const char *method,const char *query_string)
{
	print_debug("enter  exe_cgi");
	int numchars=0;
	int content_length=-1;
	char buf[_COMM_SIZE_];
	int cgi_input[2]={0,0};//
    int cgi_output[2]={0,0};
	
	if(strcasecmp(method,"GET")==0){
		clear_header(sock_client);
	}else{//POST
		do{
			memset(buf, '\0', sizeof(buf));
			numchars = get_line(sock_client, buf, sizeof(buf));
			if(strncasecmp(buf, "Content-Length:", strlen("Content-Length:")) == 0){
				content_length = atoi(&buf[16]);
			}
		}while(numchars > 0 && strcmp(buf, "\n") != 0);
			if(content_length==-1){
				echo_error_to_client(sock_client,400);
				return ;
			}
	// }  //no die i'm fail			
	//	numchars=get_line(sock_client,buf,sizeof(buf));
	//	while((numchars>0)&&(strcmp(buf,"\n"))!=0){
	//		//buf[15]='\0';	
	//		memset(buf,'\0',sizeof(buf));
	//		numchars=get_line(sock_client,buf,sizeof(buf));
	//		if(strncasecmp(buf,"Content-Length:",strlen("Content-Length:"))==0){
	//			content_length=atoi(&buf[16]);
	//		}
	//	//}//  i'm die
	//		if(content_length==-1){
	//		//	echo_error_to_client(sock_client,400);
	//			return ;
	//		}
	//   // }  //no die i'm fail			
	}
	
	print_debug("begin pipe create");
	memset(buf,'\0',sizeof(buf));
	//sprintf(buf,"HTTP/1.0 200 OK\r\n");
	strcpy(buf,HTTP_VERSION);
	strcat(buf," 200  OK\r\n\r\n");//
	send(sock_client,buf,strlen(buf),0);	
	if(pipe(cgi_input)==-1){
		echo_error_to_client(sock_client,401);
		return ;
	}
	if(pipe(cgi_output)==-1){
		close(cgi_input[0]);
		close(cgi_input[1]);
		echo_error_to_client(sock_client,401);
		return ;
	}
	print_debug("pipe is success");
	pid_t id;
	id=fork();
//	printf("id :%d\n",id);
	if(id<0){
		print_debug("fork fail");
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		echo_error_to_client(sock_client,401);
		return;
	}
	 if(id==0){//child		
		print_debug("enter child");
		
		char method_env[_COMM_SIZE_];
		memset(method_env,'\0',sizeof(method_env));
		char query_env[_COMM_SIZE_/10];
		memset(query_env,'\0',sizeof(query_env));	
		char length_env[_COMM_SIZE_];
		memset(length_env,'\0',sizeof(length_env));	
	
		close(cgi_input[1]);
		close(cgi_output[0]);
		
		dup2(cgi_input[0],0);//close(0),cgi_input[0]=0
		dup2(cgi_output[1],1);
		
		sprintf(method_env,"REQUEST_METHOD=%s",method);
		putenv(method_env);

		if(strcasecmp("GET",method)==0){//GET
			sprintf(query_env,"QUERY_STRING=%s",query_string);	
			putenv(query_env);
		}else{//POST
	//		sprintf(length_env,"CONTENT_LENGTH=%s",content_length);
			sprintf(length_env,"CONTENT_LENGTH=%d",content_length);
			putenv(length_env);
		}
		execl(path,path,NULL);//cgi
		//print_debug("child is success");
		exit(1);

	}else{//father
		print_debug("father is begin");
		close(cgi_input[0]);
		close(cgi_output[1]);
		int i=0;
		char c='\0';
		if(strcasecmp("POST",method)==0){
			for(;i<content_length;++i){
				recv(sock_client,&c,1,0);
				write(cgi_input[1],&c,1);
			}
		}
		while(read(cgi_output[0],&c,1)>0){
			send(sock_client,&c,1,0);
		
		}
		
		close(cgi_input[1]);
		close(cgi_output[0]);
		waitpid(id,NULL,0);
	}
}

void *accept_request(void *arg)
{
	print_debug("get a new connect...");
	pthread_detach(pthread_self());
	char path[_COMM_SIZE_];//1024
	char method[_COMM_SIZE_/10];
	char url[_COMM_SIZE_];
	char buffer[_COMM_SIZE_];
	memset(path,'\0',sizeof(path));
	memset(url,'\0',sizeof(url));
	memset(buffer,'\0',sizeof(buffer));
	memset(method,'\0',sizeof(method));

	int cgi=0;
	int sock_client=(int)arg;
        char * query_string=NULL;

	if(get_line(sock_client,buffer,sizeof(buffer))<0){
		return (void*)-1;	
	}
	int i=0;
	int j=0;
	
	while((!isspace(buffer[j]))&&(i<sizeof(method)-1)&&(j<sizeof(buffer))){
		method[i]=buffer[j];
		i++;
		j++;

	}
	method[i]='\0';
	printf("\n");
	printf("method :%s\n",method);
	//  no GET or POST
	 if(strcasecmp(method,"GET")&&strcasecmp(method,"POST")){
//		print_debug("method is wrong\n");
		echo_error_to_client(sock_client,501);
		return (void*)-1;
	}
	while((isspace(buffer[j])&&(j<sizeof(buffer)))){
		j++;
	}
	//get url
	i=0;
	while((!isspace(buffer[j]))&&(i<sizeof(url)-1)&&(j<sizeof(buffer))){
		url[i]=buffer[j];
		i++,j++;
	}
	url[i]='\0';
	printf("url    :%s\n",url);
	if(strcasecmp(method,"POST")==0){
		cgi=1;
	}
	
	if(strcasecmp(method,"GET")==0){
		query_string=url;
		while((*query_string!='?') && (*query_string!='\0')){
			query_string++;
		}
		if(*query_string=='?'){
			*query_string='\0';
			query_string++;
			cgi=1;
		}
	}

	sprintf(path,"htdocs%s",url);
	if(path[strlen(path)-1]=='/'){
		strcat(path,MAIN_PAGE);//index.html
		//strcat(path,"index.html");	
	}
	printf("path   :%s\n",path);
	struct stat st;
	if(stat(path,&st)<0){//fail
		print_debug("miss cgi");
		clear_header(sock_client);	
		echo_error_to_client(sock_client,501);
	}else{
		if((st.st_mode&S_IFMT)==S_IFDIR){
			strcat(path,"/");
			strcat(path,MAIN_PAGE);//index.html
			//strcat(path,"index.html");
		}else if((st.st_mode&S_IXUSR) || (st.st_mode&S_IXGRP) || (st.st_mode&S_IXOTH)){
			cgi=1;
		}else{
			//
		}
		if(cgi){
			printf("query string :%s\n",query_string);
		//	exe_cgi(sock_client,method,path,query_string);//is low wrong
			exe_cgi(sock_client,path,method,query_string);
		}else{
			clear_header(sock_client);
			print_debug("begin enter our echo_html");
			echo_html(sock_client,path,st.st_size);
			print_debug("echo_html is success");
		}
	}
	close(sock_client);
	return NULL;
}
void usage(const char *port)
{
	printf("usage [ip][port]\n",port);
}
int start(int* port)
{
	int listen_sock=0;
	//int flag = 1;
	//setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));
	listen_sock=socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock==-1){
		error_die("socket");
		exit(1);
	}
	//reuse port
	int flag = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag));
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(*port);
	local.sin_addr.s_addr=htonl(INADDR_ANY);
	socklen_t len=sizeof(local);

	if(bind(listen_sock,(struct sockaddr*)&local,len)==-1){
  		error_die("bind");
		exit(2);
	}
	if(listen(listen_sock,5)<0){
		error_die("");
		exit(3);
	}
	return listen_sock;
}
//./httpd ip[] port
int main(int argc,char *argv[])
{
	if(argc!=3){
		usage(argv[0]);
	exit(1);
	}
	int port=atoi(argv[2]);
	int sock=start(&port);
	print_debug("start is success");	

	printf("httpd running on port :%d\n",port);	
	
	struct sockaddr_in client;
	int  len = sizeof(client);
	while(1){
		int  new_sock=accept(sock,(struct sockaddr*)&client,&len);
		print_debug("accept is success ");
		if(new_sock<0){
			error_die("new_sock");
			
			continue;
		}
		pthread_t new_thread;
		print_debug("new_sock is success");
		pthread_create(&new_thread,NULL,accept_request,(void *)new_sock);
		print_debug("pthread_create is success");	
	}
	close(sock);
	return 0;
}
