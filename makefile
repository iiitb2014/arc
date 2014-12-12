all: server.c
	gcc server.c -o server
clean: users connected-users
	rm users connected-users chat*
