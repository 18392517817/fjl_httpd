#*************************************************************************
#	> File Name: Makefile
#	> Author: fjl_57
#	> Mail: 799619622@qq.com 
#	> Created Time: Sun 06 Mar 2016 10:05:55 PM EST
#************************************************************************/
PWD=$(shell pwd)
CGI_PATH=$(PWD)/cgi_bin 
SER_BIN=myhttpd
CLI_BIN=demo_client
SER_SRC=myhttpd.c
CLI_SRC=demo_client.c
INCLUDE=.
CC=gcc
FLAGS=-o
LDFLAGS=-lpthread #-static
LIB=

.PHONY:all
all:$(SER_BIN) $(CLI_BIN) cgi

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)-D_EPOLL_ #-D_DEBUG_ _# -D_PTHREAD_
$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)

.PHONY:cgi
cgi:
	@for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make;\
		cd -;\
	done

.PHONY:output
output: all
	@mkdir -p output/htdocs/cgi_bin
	@cp myhttpd output 
	@cp demo_client output
	@cp -rf conf output
	@cp -rf log output
	@cp start.sh output
	@cp -rf htdocs/* output/htdocs
	@for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make output;\
		cd -;\
	done
.PHONY:clean
clean:
	@rm -rf $(SER_BIN) $(CLI_BIN) output
	@for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make clean;\
		cd -;\
	done
