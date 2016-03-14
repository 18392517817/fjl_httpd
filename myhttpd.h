
/*************************************************************************
	> File Name: myhttpd.c
	> Author: fjl_57
	> Mail: 799619622@qq.com 
	> Created Time: Sun 06 Mar 2016 10:05:55 PM EST
************************************************************************/

#ifndef _HTTPD_H_
#define _HTTPD_H_
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<pthread.h>
#include<sys/wait.h>
#include<sys/stat.h>
//#include<string>
#include<ctype.h>
#include<unistd.h>
#include<fcntl.h>
#define MAIN_PAGE "index.html"

#define HTTP_VERSION "HTTP/1.0"

void clear_header(int sock_client);
void error_die(const char *sc);
int get_line(int sock,char *buf,int max_len);
static void bad_request(int client);
static void cannot_execute(int client);
static void server_unavibale(int client);
static void not_found(int client);
void echo_error_to_client(int client, int error_code);
int start(int* port);
void usage(const char *proc);
void *accept_request(void *arg);
void echo_html(int client,const char *path,unsigned int file_size);
void echo_error_to_client(int client, int error_code);
void exe_cgi(int sock_client,const char *path,const char *method,const char *query_string);
#endif


