#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void math_div(char *const data_string)//data1=XXX&data2=YYY;
{
	if(!data_string){
		return;
	}
	char *data1 = NULL;
	char *data2 = NULL;
	char *start = data_string;
	while(*start != '\0'){
		if(*start == '=' && data1 == NULL){
			data1 = start+1;
			start++;
			continue;
		}
		if(*start == '&'){
			*start = '\0';
		}
		if(*start == '=' && data1 != NULL){
			data2 = start+1;
			break;
		}
		start++;
	}
	float int_data1 = atoi(data1);
	float int_data2 = atoi(data2);
	if(int_data2==0){
		printf("<p>dividend is 0,parameter is error: </p>\n");
		return ;
	}
	float add_res = int_data1/int_data2;
	printf("<p>math [div] result : %f</p>\n", add_res);
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

	printf("<html>\n");
	printf("<head>MATH</head>\n");
	printf("<body>\n");
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET", method) == 0 ){
		strcpy(query_string, getenv("QUERY_STRING"));
		math_div(query_string);//data1=XXX&data2=YYY;
	}else if( strcasecmp("POST", method) == 0 ){
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0; 
		for(; i < content_length; i++ ){
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		math_div(post_data);//data1=XXX&data2=YYY;
	}else{
		//DO NOTHING
		return 1;
	}

	printf("</body>\n");
	printf("</html>\n");
}
