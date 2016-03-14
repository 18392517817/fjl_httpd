/***********************************************************************
	> File Name: register.c
	> Author: fjl_57
	> Mail: 799619622@qq.com 
	> Created Time: Sun 06 Mar 2016 10:05:55 PM EST
************************************************************************/
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "sql_connect.h" //sql_connect.h

const std::string _remote_ip = "127.0.0.1";
const std::string _remote_user = "myuser";
const std::string _remote_passwd = "mypassword";
const std::string _remote_db   = "bit_five";


void reg_table(char *const data_string)  //char *const data_string
{
	
//	printf("enter reg_table\n");
	if(!data_string){
		std::cout<<"data_string is NULL"<<std::endl;
		return;
	}
//	char *name = NULL;
//	char *age = NULL;
//	char *school = NULL;
//	char *hobby = NULL;
//	char *start = data_string;
//	char post_data[4096];
//    memset(post_data, '\0', sizeof(post_data));
//	while(*start != '\0'){
//		if(*start == '=' && name== NULL){
//			name = start+1;
//			start++;
//		}
//		if(*start == '&'){
//			*start = '\0';
//			start++;
//		}
//		if(*start == '=' && age== NULL){
//			age = start+1;
//			start++;
//		}
//		if(*start == '&'){
//			*start = '\0';
//			start++;
//		}
//		if(*start=='=' && school==NULL){
//			school=start+1;
//			start++;
//		}
//		if(*start == '&'){
//			*start = '\0';
//			start++;
//		}
//		if(*start=='=' && hobby==NULL){
//			hobby=start+1;
//			start++;
//			break;
//		}
//
//		start++;
//	}
//	
//
//
//	//	printf("name=%s,age=%s,school=%s,hobby=%s\n",name,age,school,hobby);	
        char name[24];
     	char age[24];
     	char school[24];
     	char hobby[24];
     	char *start_string = data_string;
		printf("start_string =%s\n",start_string);
     	char post_data[4096];
         memset(post_data, '\0', sizeof(post_data));
 
	 int i=0;
    while(start_string[i]!='\0'){
    	if((start_string[i]=='=') || (start_string[i]=='&')){
    		start_string[i]=' ';
    	}
    	i++;
    }
    bzero(name,sizeof(name));//beffor n is \0
    bzero(age,sizeof(age));
    bzero(school,sizeof(school));
    bzero(hobby,sizeof(hobby));
    //data_string     name qq age 12 school qh hobby code
	sscanf(start_string, "%*s %s %*s %s %*s %s %*s %s",name, age, school, hobby);
	
//	printf("scscanf after start_string =%s\n",start_string);
//	printf("name=%s\t",name);
//	printf("age=%s\t",age);
//	printf("school=%s\t",school);
//	printf("hobby=%s\t",hobby);
	
	    const std::string _host = _remote_ip;
    	const std::string _user = _remote_user;
    	const std::string _passwd = _remote_passwd;
    	const std::string _db   = _remote_db;
    	sql_connecter conn(_host, _user, _passwd, _db);
		
    	conn.begin_connect();
		//strcat(post_data,"\"\'");	 //no use
		strcat(post_data,"\'");	
		strcat(post_data,name);	
		strcat(post_data,"\'");	
		strcat(post_data,",");	
		strcat(post_data,age);	
		strcat(post_data,",\'");	
		strcat(post_data,school);	
		strcat(post_data,"\',");	
		strcat(post_data,"\'");	
		strcat(post_data,hobby);	
		strcat(post_data,"\'");
	//	printf("post_data=%s\n",post_data);	
	    conn.insert_sql(post_data);//"'fjl',22,'xgy','code'"
		printf("<p>insert success! </p>\n");
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));

	std::cout<<"<html>"<<std::endl;
	std::cout<<"<head>Insert message</head>"<<std::endl;
	std::cout<<"<body>"<<std::endl;
	
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET", method) == 0 ){
		strcpy(query_string, getenv("QUERY_STRING"));
		reg_table(query_string);//name=qq&age=13&school=qh&hoppy=code
	}else if( strcasecmp("POST", method) == 0 ){
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0; 
		for(; i < content_length; i++ ){
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		reg_table(post_data);//name=qq&age=13&school=qh&hoppy=code
	}else{
		//DO NOTHING
	//	printf("method is wrong\n");
		return 1;
	}
	std::cout<<"</body>"<<std::endl;
	std::cout<<"</html>"<<std::endl;
}
