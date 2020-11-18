// socket client multi clients
// File: client.c
// compile: gcc -Wall client.c -o client
// run: ./client localhost
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include <unistd.h>        	          // for read/write/close
#include <sys/types.h>     	          /* standard system types        */
#include <netinet/in.h>    	          /* Internet address structures */
#include <sys/socket.h>   	          /* socket interface functions  */
#include <netdb.h>         	          /* host to IP resolution            */

#define  BUFLEN        20          /* maximum response size     */

void error(char* str){
    perror(str);
    exit(EXIT_FAILURE) ;
}

int main(int argc, char *argv[])
{
    int                 rc;            /* system calls return value storage */
    int                 sockfd;
    int                 num;
    char                rbuf[BUFLEN];
    struct sockaddr_in serv_addr;
    struct hostent *server;


    if (argc < 3) {
        fprintf(stderr, "Missing server name \n");
        exit(EXIT_FAILURE) ; 
    }

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("socket failed");


    // connect to server
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n"); 
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(argv[2]));

    rc = connect(sockfd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0) 
        error("connect failed:");
    

    while(1){
	    puts("enter a number");
	    scanf("%d",&num);
	    if(!num) break;
	    sprintf(rbuf,"%d",num);

	    // send and then receive messages from the server
	    write(sockfd, rbuf, strlen(rbuf)+1) ;
	    rc = read(sockfd, rbuf,  BUFLEN) ;
        if(rc > 0)
	    	  printf("%s\n", rbuf) ;
	    else 
              error("read() failed") ;

             
	    
    }

    close(sockfd);

    return EXIT_SUCCESS;
}



