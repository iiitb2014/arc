#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

#define CONNMAX 100
#define BYTES 1024
char *ROOT;
int listenfd, clients[CONNMAX];
void error(char *);
void startServer(char *);
void respond(int);
void Connected();
int updateFile(int connect_id,char *msg);
int main(int argc, char* argv[])
{
  int loop;
  struct sockaddr_in clientaddr;
  socklen_t addrlen;
  char c;

  //Default Values PATH = ~/ and PORT=10000
  char PORT[6];
  ROOT = getenv("PWD");
  strcpy(PORT,"10000");

  int slot=0;

  //Parsing the command line arguments
  while ((c = getopt (argc, argv, "p:r:")) != -1)
    switch (c)
  {
    case 'r':
    ROOT = malloc(strlen(optarg));
    strcpy(ROOT,optarg);
    break;
    case 'p':
    strcpy(PORT,optarg);
    break;
    case '?':
    fprintf(stderr,"Wrong arguments given!!!\n");
    exit(1);
    default:
    exit(1);
  }

  printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");
  // Setting all elements to -1: signifies there is no client connected
  int i;
  for (i=0; i<CONNMAX; i++)
    clients[i]=-1;
  startServer(PORT);

  // ACCEPT connections
  while (1)
  {
    addrlen = sizeof(clientaddr);
    clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

    if (clients[slot]<0)
      error ("accept() error");
    else
    {
      if ( fork()==0 )
      {
        respond(slot);
        exit(0);
      }
    }
    printf("%d here\n",slot);
    while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
  }
  delete("chat");
  return 0;
}

//start server
void startServer(char *port)
{
  struct addrinfo hints, *res, *p;

  // getaddrinfo for host
  memset (&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo( NULL, port, &hints, &res) != 0)
  {
    perror ("getaddrinfo() error");
    exit(1);
  }
  // socket and bind
  for (p = res; p!=NULL; p=p->ai_next)
  {
    listenfd = socket (p->ai_family, p->ai_socktype, 0);
    if (listenfd == -1) continue;
    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
  }
  if (p==NULL)
  {
    perror ("socket() or bind()");
    exit(1);
  }

  freeaddrinfo(res);

  // listen for incoming connections
  if ( listen (listenfd, 1000000) != 0 )
  {
    perror("listen() error");
    exit(1);
  }
}

int pushID(char *string)
	{
	int i=0;
  char str[999];
  FILE *in = fopen("users","a+");
	while(fscanf(in,"%s",str)==1)
		if(strcmp(str,string)==0)
			return -1;
  fprintf(in,"%s\n",string);
  fclose(in);
  in = fopen("users","r");
  for(i=0;fscanf(in,"%s\n",str)==1;++i);
  fclose(in);
  if(i>1)
  {
    Connected();
    remove("users");
  }
  return 0;
	}
//spilts the id based on delimitor
int found(char *u_id,char *it)
{
  char *s;
  s=strtok(u_id,"-");
  if(strcmp(s,it)==0)
    return 1;
  else
  {
    s=strtok(NULL,"-");
    if(strcmp(s,it)==0)
      return 1;
  }
  return 0;
}

//searches the unique array for the id given
int getConnected(char *item)
{
  int i=0;
  char str[999];
  FILE *in = fopen("connected-users","r");
	while(fscanf(in,"%s",str)==1)
  {
    if (found(str,item))
	    return i;
    i++;
  }
  return -1;
}

//adds concatenated id's to the unique_id array
void Connected()
{
  FILE *in = fopen("users","r");
  FILE *out = fopen("connected-users","a+");
  char ID1[999], ID2[999];
  fscanf(in,"%s\n",ID1);
  fscanf(in,"%s\n",ID2);
  fprintf(out,"%s-%s\n",ID1,ID2);
  fclose(in);
  fclose(out);
}

char *getFilename(int connect_id){
	char *filename = (char*)malloc(20);
	snprintf(filename,20,"chat%d",connect_id);
	return filename;
}

char *getID(char *msg)
{	
	char *tmp = (char*)malloc(999);
	char *tmpID = strtok(msg,"&");
    	char *ID = strtok(tmpID,"=");
    	ID = strtok(NULL,"\0");
	strcpy(tmp,ID);
	return tmp;
}

	
char *formatPOST(char *msg){
	char *tmp = (char*)malloc(999);
	char *tmpID = strtok(msg,"&");
    	char *tmptext = strtok(NULL,"&");
	if(strcmp(tmptext,"\0")==0){
		strcpy(tmp,"\0");
		return tmp;
	}
    	char *tmptime = strtok(NULL, "\0");
    	char *ID = strtok(tmpID,"=");
    	ID = strtok(NULL,"\0");
    	char *text = strtok(tmptext,"=");
    	text = strtok(NULL,"\0");
    	char *time = strtok(tmptime,"=");
    	time = strtok(NULL,"\0");
	snprintf(tmp,999,"<li class=\"msg\"><span id=\"user\">%s</span>%s<span class=\"right\">%s</span>",ID,text,time);
	return tmp;
}
//function which creates & updates the file
int updateFile(int connect_id, char *msg)
{
  char *filename = getFilename(connect_id);
  if(msg[0]!='\0'){
    FILE *in;
    in=fopen(filename,"a+");
    //tmp = ;
    char *tmp = formatPOST(msg);
    if(strcmp(tmp,"\0")!=0)
	    fprintf(in,"%s\n",tmp);
    fclose(in);
    return 1;
  }
  return 0;
}
//client connection
void respond(int n)
{
  char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
  int rcvd, fd, bytes_read;

  memset( (void*)mesg, (int)'\0', 99999 );

  rcvd=recv(clients[n], mesg, 99999, 0);

  if (rcvd<0)    // receive error
    fprintf(stderr,("recv() error\n"));
  else if (rcvd==0)    // receive socket closed
    fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    // message received
  {
    printf("%s", mesg);
    reqline[0] = strtok (mesg, " \t\n");
    if ( strncmp(reqline[0], "GET\0", 4)==0 )
    {
      reqline[1] = strtok (NULL, " \t");
      reqline[2] = strtok (NULL, " \t\n");
      if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
      {
        write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
      }
      else
      {	char str[999];
	strcpy(str,reqline[1]);
        if ( strncmp(reqline[1], "/\0", 2)==0 )
          reqline[1] = "/index.html";        //Because if no file is specified, index.html will be opened by default (like it happens in APACHE...
	if(strncmp(str,"/chat",5)==0)
	{		
	printf("hello");
	char *str = strtok(reqline[1],"$");
        char *req = strtok(NULL, "\0");
		printf("%s",req);
		strcpy(str,req);
		char *ID = getID(req);
		pushID(ID);
		printf("\n$%s$\n",str);
		int connect_id = getConnected(ID);
          if(connect_id!=-1)
	  {
                printf("connect_id=%d",connect_id);
        	strcpy(path, ROOT);
	        strcat(path, "/");
	        strcpy(&path[strlen(ROOT)+1], getFilename(connect_id));
          }
	}
	else
	{
		strcpy(path, ROOT);
		strcpy(&path[strlen(ROOT)], reqline[1]);
        }
	printf("file: %s\n", path);

        if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND
        {
          send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
          while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
            write (clients[n], data_to_send, bytes_read);
        }
        else    write(clients[n], "HTTP/1.0 404 Not Found\n", 23); //FILE NOT FOUND
      }
    }
    else if ( strncmp(reqline[0], "POST\0", 5)==0 )
    {
      reqline[1] = strtok (NULL, " \t");
      reqline[2] = strtok (NULL, " \t\n");
      if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
      {
        write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
      }
      else
      {
        char *str = strtok(NULL,"$");
        char *req = strtok(NULL, "\0");
	if(strncmp(reqline[1],"/chat",5)==0)
	{	printf("el\n");
		strcpy(str,req);
		char *ID = getID(req);
		printf("#id = %s \n",ID);
		pushID(ID);
		int connect_id = getConnected(ID);
          if(connect_id!=-1)
	  {
            printf("connect_id=%d",connect_id);
	        updateFile(connect_id, str);
        	strcpy(path, ROOT);
        	strcat(path, "/");
	        strcpy(&path[strlen(ROOT)+1], getFilename(connect_id));
          }
	}
	else
	{
        strcpy(path, ROOT);
        strcpy(&path[strlen(ROOT)], reqline[1]);
	}
        printf("file: %s\n", path);
        if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND
        {
          send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
          while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
            write (clients[n], data_to_send, bytes_read);
        }
        else    write(clients[n], "HTTP/1.0 404 Not Found\n", 23); //FILE NOT FOUND
      }
    }
  }
  //Closing SOCKET
  shutdown (clients[n], SHUT_RDWR);         //All further send and recieve operations are DISABLED...
  close(clients[n]);
  clients[n]=-1;
}
