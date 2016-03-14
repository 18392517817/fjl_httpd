
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
int main()
{

		
	char *name = "fjl";
	char *age = "23";
	char *school = "xgd";
	char *hobby = "code";
	char post_data[4096];
    memset(post_data, '\0', sizeof(post_data));
		printf("name=%s,age=%s,school=%s,hobby=%s\n",name,age,school,hobby);	
		strcat(post_data,"\" \'");	
		strcat(post_data,name);	
		strcat(post_data,"\'");	
		strcat(post_data,",");	
		strcat(post_data,age);	
		strcat(post_data,",\'");	
		strcat(post_data,school);	
		strcat(post_data,"\',");	
		strcat(post_data,"\'");	
		strcat(post_data,hobby);	
		strcat(post_data,"\'\"");
		printf("post_data=%s\n",post_data);
}
