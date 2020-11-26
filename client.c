// socket client multi clients
// File: client.c
// compile: gcc -Wall client.c -o client
// run: ./client localhost
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for read/write/close
#include <sys/types.h>  /* standard system types        */
#include <netinet/in.h> /* Internet address structures */
#include <sys/socket.h> /* socket interface functions  */
#include <netdb.h>      /* host to IP resolution            */
#define BUFLEN 20       /* maximum response size     */

void error(char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int r = -1; //we don't have 'r'
    int p = -1; //we don't have 'p'
    int isPort = -1;
    int isPath = -1;
    char *path;
    char *host;
    char *port;
    /* CHECK IT HERE*/
    char *parameter = (char *)malloc(sizeof(char) * 1020); //with &
    char *txt;
    char *buf = (char *)malloc(sizeof(char) * 1020); //the get or post request
    int intPort = 80;

    for (int i = 1; i < argc; i++) /*for loop on all the string i = 1  -> bcz 0 is ./client*/
    {
        int inputLen = strlen(argv[i]); //the length of the word
        char *token;
        char *rest = (char *)malloc(sizeof(char) * inputLen);
        strcpy(rest, argv[i]);
        char *ptr = strstr(rest, "http://"); //check if it has "https://" in the
        if (ptr != NULL)
        {
            ptr = (ptr + 7);
            host = ptr; //we have just the hos

            char *secPtr = strstr(ptr, ":");
            if (secPtr != NULL) //there is port
            {
                isPort = 1;
                secPtr = secPtr + 1;
                host = secPtr - ptr; //this is 14 i should just copy 14 char s
                int num = (int *)host;
                host = (char *)malloc(num);
                strncpy(host, ptr, num); //-1
                port = (char *)malloc(strlen(secPtr));
                strncpy(port, secPtr, strlen(secPtr) - 1);
            }
            else //there isn't a port
            {
                secPtr = ptr;
            }
            char *thrdPtr = strstr(secPtr, "/");
            if (thrdPtr != NULL) //there is a path
            {
                isPath = 1;
                path = thrdPtr;
                if (isPort == 1)
                {
                    int num1 = thrdPtr - secPtr;
                    port = (char *)malloc(num1);
                    strncpy(port, secPtr, num1);
                }
                else
                {
                    host = thrdPtr - ptr;
                    int num = (int *)host;
                    host = (char *)malloc(num);
                    strncpy(host, ptr, num); //-1
                }
            }
            else //there isn't a path
            {
                path = (char *)malloc(1); //i should make malloc
                strcpy(path, "/");
            }
            if (host != NULL)
                printf("the host is:%s\n", host);
            if (isPort != -1)
                printf("the port is:%s\n", port);

            printf("the path is:%s\n", path);

            continue; // we have finished testing the argv[i] go to the second
        }
        ptr = strstr(rest, "-p");
        if (ptr != NULL)
        {          //it's equal to -p
            p = 1; // we have a post request
            if (i + 1 >= argc)
            {
                printf("thre is no text");
            }
            if (argv[i + 1] != NULL)
            {
                int txtLength = strlen(argv[i + 1]);
                char *rest = (char *)malloc(sizeof(char) * txtLength);
                strcpy(rest, argv[i + 1]);
                i = i + 1; //discard the text of the  -p
                printf("%s\n", rest);
                txt = rest; ///////////////////////
            }
            else
            {
                printf("there is no text");
            }
            continue;
        }
        ptr = strstr(rest, "-r");
        if (ptr != NULL)
        {

            if (i + 1 >= argc)
            {
                printf("thre is no number");
            }
            if (argv[i + 1] != NULL)
            {
                int myint1 = atoi(argv[i + 1]);
                r = myint1; //there is a parameter
                for (int j = 0; j < r; j++)
                {
                    if (i + 1 >= argc)
                    {
                        printf("thre is no number");
                    }
                    i = i + 1;

                    if (j == 0)
                    {
                        strcat(parameter, "?");
                        strcat(parameter, argv[i + 1]);
                    }
                    else
                    {
                        strcat(parameter, "&");
                        strcat(parameter, argv[i + 1]);
                    }
                }
            }
        }
    }

    if (isPort != -1)
    {
        intPort = atoi(port);
    }

    int reqSize; // the size of the request malloc

    char *request; //i should malloc to the request the size of it should be the equal to the bottom -> make it geniric  .
                   //we use it just for the write to the server
                   //GET /index.html HTTP/1.0\r\nHost: www.jce.ac.il\r\n\r\n

    int rc; ///* system calls return value storage
    int sockfd;
    int num;
    char rbuf[BUFLEN];
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc <= 1)
    {
        fprintf(stderr, "Missing server name \n");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("socket failed");

    // connect to server
    server = gethostbyname(host); //put the IP here
    //printf("\nww%sww",host);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(intPort); //put the port here

    rc = connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (rc < 0)
        error("connect failed:");
    else
    {
        printf("connected\n");
    }

    if (p == -1) //get request
    {
        strcat(buf, "GET ");
    }
    else
    {
        strcat(buf, "POST "); //shof al text
    }
    strcat(buf, path);
    strcat(buf, parameter);
    strcat(buf, " HTTP/1.0\r\nHost: ");
    strcat(buf, host);
    // if(p != -1){
    //     strcat(buf , )
    // }
    if (p != -1)
    {
        strcat(buf, "\r\n");
        strcat(buf,"Content-length:");
        char h [strlen(txt)] ; 
        sprintf(h,"%d" , strlen(txt));
        strcat(buf,h  );

    }
    strcat(buf, "\r\n\r\n");
    if (p != -1)
    {
        strcat(buf, txt);
    }
    
    printf("\n%s\n", buf);
    //you should build a http request here   //we make a concatinate if it get or post then ....
    //rbuf is a the http massage-> GET /index.html HTTP/1.0\r\nHost: www.jce.ac.il\r\n\r\n
    //this should be the massage thn we write the rbuf to the server ""but i should make it geniric ""
    //make a function httpRequest(rbuf,argv);

    //before the write i should print the request "the get request "
    // send and then receive messages from the server
    //  strcpy(buf,"GET /index.html HTTP/1.0\r\nHost: www.jce.ac.il\r\n\r\n");
    write(sockfd, buf, strlen(buf) + 1); //send the massage to the server -> socket
    // for(){} , here we should read the massage using a loop
    do
    {
        rc = read(sockfd, rbuf, BUFLEN); //read from the sever the massage
        if (rc > 0)
        { //i should to the rbuf in the place of rc rbuf[rc]= '\0'
            printf("%s\n", rbuf);
            rbuf[rc] = '\0';
        }
        else
            error("read() failed");
        //here the end of the for
    } while (rc > 0);

    close(sockfd);

    return EXIT_SUCCESS;
}
