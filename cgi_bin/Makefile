FIND_PATH=-I/home/fujl/fjl/fjl_httpd/sql_connect -I/home/fujl/fjl/fjl_httpd/sql_connect/mysql_lib/include -L/home/fujl/fjl/fjl_httpd/sql_connect/mysql_lib/lib -lmysqlclient

.PHONY:all
all: debug_cgi math_cgi div_cgi show_student_cgi register_cgi
debug_cgi:debug_cgi.c
	@gcc -o $@ $^
math_cgi:math_cgi.c
	@gcc -o $@ $^
div_cgi:div_cgi.c
	@gcc -o $@ $^
show_student_cgi:show_student_cgi.cpp /home/fujl/fjl/fjl_httpd/sql_connect/sql_connect.cpp 
	@g++ -o $@ $^ $(FIND_PATH) -g

register_cgi:register_cgi.cpp /home/fujl/fjl/fjl_httpd/sql_connect/sql_connect.cpp 
	@g++ -o $@ $^ $(FIND_PATH)


.PHONY:output
output:
	@cp debug_cgi ../output/htdocs/cgi_bin 
	@cp math_cgi ../output/htdocs/cgi_bin 
	@cp div_cgi ../output/htdocs/cgi_bin 
	@cp show_student_cgi ../output/htdocs/cgi_bin
	@cp register_cgi ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	@rm -rf debug_cgi math_cgi div_cgi show_student_cgi register_cgi
