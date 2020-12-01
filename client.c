#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for read/write/close
#include <sys/types.h>  /* standard system types        */
#include <netinet/in.h> /* Internet address structures */
#include <sys/socket.h> /* socket interface functions  */
#include <netdb.h>      /* host to IP resolution            */
#define BUFLEN 20       /* maximum response size     */
/*system call error*/
void error(char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
    printf("\n");
}
/*too little argument*/
void argcError(int argc)
{
    if (argc <= 1)
    {
        fprintf(stderr, "Missing server name \n");
        exit(EXIT_FAILURE);
    }
}
/*usage error*/
void UsageError()
{
    printf("Usage: client [-p <text>] [-r n < pr1=value1 pr2=value2 …>] <URL>");
    exit(0);
}
void Nfree(char *s)
{
    if (s != NULL)
    {
        free(s);
    }
    else
    {
    }
}
/*form the http request and send it to the server*/
void request(int argc, char *argv[])
{
    int r = -1;
    int p = -1;
    int isPort = -1;
    char *path = NULL;
    char *host = NULL;
    char *port = NULL;
    char *parameter = NULL;
    int hostL = 0;
    int portL = 0;
    int pathL = 0;
    char *txt = NULL;
    char *buf = NULL;
    int intPort = 80;
    int hostLength = 0;
    int portLength = 0;
    char *newHost = NULL;
    char *newPort = NULL;

    for (int i = 1; i < argc; i++)
    {
        char *ptr = strstr(argv[i], "http://");
        /*if we have found the server name*/
        if (ptr != NULL)
        {
            ptr = ptr + 7;
            host = ptr;
            hostL = strlen(host);
            char *ptr2 = strstr(host, ":");

            if (ptr2 != NULL)
            {
                ptr2 = ptr2 + 1;
                port = ptr2;
                isPort = 1;
                portL = strlen(port);
            }
            else
            {
                ptr2 = host;
            }

            char *ptr3 = strstr(ptr2, "/");
            if (ptr3 != NULL)
            {
                path = (char *)malloc(strlen(ptr3) + 1);
                for (int g = 0; g < strlen(ptr3) + 1; g++)
                {
                    path[g] = '\0';
                }
                strcpy(path, ptr3);
                // path = ptr3;

                pathL = strlen(path);
            }
            else
            {
                path = (char *)malloc(1 + 1);
                for (int g = 0; g < 2; g++)
                {
                    path[g] = '\0';
                }
                strcpy(path, "/");
            }
            hostLength = hostL - portL - 1; //-1
            portLength = portL - pathL;
            if (portLength == 0)
            {
                Nfree(path);
                Nfree(newHost);
                Nfree(newPort);

                Nfree(parameter);
                // Nfree(rbuf);

                Nfree(buf);
                Nfree(txt);
                UsageError();
            }
            if (isPort == -1)
            {
                hostLength = hostL - pathL;
            }
            newHost = (char *)malloc(hostL + 1);
            for (int g = 0; g < hostL + 1; g++)
            {
                newHost[g] = '\0';
            }

            if (newHost == NULL)
            {
                Nfree(path);
                Nfree(newHost);
                Nfree(newPort);

                Nfree(parameter);
                // Nfree(rbuf);

                Nfree(buf);
                Nfree(txt);
                error("malloc error");
            }
            strncpy(newHost, host, hostLength);

            if (isPort != -1)
            {
                newPort = (char *)malloc(portL + 1);
                for (int g = 0; g < portL + 1; g++)
                {
                    newPort[g] = '\0';
                }
                if (newPort == NULL)
                {
                    Nfree(path);
                    Nfree(newHost);
                    Nfree(newPort);

                    Nfree(parameter);
                    //Nfree(rbuf);

                    Nfree(buf);
                    Nfree(txt);
                    error("malloc error");
                }
                strncpy(newPort, port, portLength);
                /*check if the port is a number*/
                for (int j = 0; j < portLength; j++)
                {
                    if (newPort[j] >= '0' && newPort[j] <= '9')
                    {
                    }
                    else
                    {
                        Nfree(path);
                        Nfree(newHost);
                        Nfree(newPort);

                        Nfree(parameter);
                        // Nfree(rbuf);

                        Nfree(buf);
                        Nfree(txt);
                        UsageError();
                    }
                }
            }
            continue;
        }

        ptr = strstr(argv[i], "-p");
        /*if we have a post request*/
        if (ptr != NULL)
        {
            p = 1;
            if (i + 1 >= argc)
            {
                Nfree(path);
                Nfree(newHost);
                Nfree(newPort);

                Nfree(parameter);
                // Nfree(rbuf);

                Nfree(buf);
                Nfree(txt);
                UsageError();
            }
            if (argv[i + 1] != NULL)
            {
                int txtLength = strlen(argv[i + 1]);
                char *rest = (char *)malloc(txtLength + 1);
                for (int f = 0; f < txtLength; f++)
                {
                    rest[f] = '\0';
                }
                if (rest == NULL)
                {
                    Nfree(path);
                    Nfree(newHost);
                    Nfree(newPort);

                    Nfree(parameter);
                    //  Nfree(rbuf);

                    Nfree(buf);
                    Nfree(txt);
                    error("malloc error");
                }
                strcpy(rest, argv[i + 1]);
                i = i + 1; //discard the text of the  -p
                //printf("%s\n", rest);
                txt = rest;
                //free(rest);
            }
            else
            {
                Nfree(path);
                Nfree(newHost);
                Nfree(newPort);

                Nfree(parameter);
                //Nfree(rbuf);

                Nfree(buf);
                Nfree(txt);
                UsageError();
            }
            continue;
        }

        ptr = strstr(argv[i], "-r");
        /*if we have a parameters*/
        if (ptr != NULL)
        {
            r = 1;

            if (i + 1 >= argc)
            {
                Nfree(path);
                Nfree(newHost);
                Nfree(newPort);

                Nfree(parameter);
                // Nfree(rbuf);

                Nfree(buf);
                Nfree(txt);
                UsageError();
            }
            if (argv[i + 1] != NULL)
            {
                for (int z = 0; z < strlen(argv[i + 1]); z++)
                {
                    if (argv[i + 1][z] >= '0' && argv[i + 1][z] <= '9')
                    {
                    }
                    else
                    {
                        Nfree(path);
                        Nfree(newHost);
                        Nfree(newPort);

                        Nfree(parameter);
                        //Nfree(rbuf);

                        Nfree(buf);
                        Nfree(txt);

                        UsageError();
                    }
                }
                int myint1 = atoi(argv[i + 1]);
                r = myint1; //there is a parameter
                int parLen = 0;
                for (int z = 0; z < r; z++)
                {
                    if (i + 2 + z >= argc)
                    {
                        Nfree(path);
                        Nfree(newHost);
                        Nfree(newPort);

                        Nfree(parameter);
                        //Nfree(rbuf);

                        Nfree(buf);
                        Nfree(txt);

                        UsageError();
                    }
                    parLen += strlen(argv[i + 2 + z]);
                }

                parameter = (char *)malloc(parLen + r + 1); //with &
                for (int g = 0; g < parLen + r + 1; g++)
                {
                    parameter[g] = '\0';
                }
                if (parameter == NULL)
                {
                    Nfree(path);
                    Nfree(newHost);
                    Nfree(newPort);

                    Nfree(parameter);
                    // Nfree(rbuf);

                    Nfree(buf);
                    Nfree(txt);

                    error("malloc error");
                }
                for (int j = 0; j < r; j++)
                {
                    if (i + 1 >= argc)
                    {
                        Nfree(path);
                        Nfree(newHost);
                        Nfree(newPort);

                        Nfree(parameter);
                        //Nfree(rbuf);

                        Nfree(buf);
                        Nfree(txt);
                        UsageError();
                    }
                    i = i + 1;
                    //check if there is = in the parameters
                    int parSize = strlen(argv[i + 1]);
                    int thereIsEqual = -1;
                    for (int y = 0; y < parSize - 1; y++)
                    {
                        if (argv[i + 1][y] == '=')
                            thereIsEqual = 1;
                        if (argv[i + 1][0] == '=' || argv[i + 1][parSize - 1] == '=')
                        {
                            Nfree(path);
                            Nfree(newHost);
                            Nfree(newPort);

                            Nfree(parameter);
                            // Nfree(rbuf);

                            Nfree(buf);
                            Nfree(txt);

                            UsageError();
                        }
                    }
                    if (thereIsEqual == -1)
                    {
                        Nfree(path);
                        Nfree(newHost);
                        Nfree(newPort);

                        Nfree(parameter);
                        // Nfree(rbuf);

                        Nfree(buf);
                        Nfree(txt);

                        UsageError();
                    }
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
                i = i + 1;
            }
            else
            {
                Nfree(path);
                Nfree(newHost);
                Nfree(newPort);

                Nfree(parameter);
                // Nfree(rbuf);

                Nfree(buf);
                Nfree(txt);

                UsageError();
            }

            continue;
        }

        if (i != argc) //-1
        {
            /////////////-1
            Nfree(path);
            Nfree(newHost);
            Nfree(newPort);

            Nfree(parameter);
            // Nfree(rbuf);

            Nfree(buf);
            Nfree(txt);

            UsageError();
        }
        //  i should check the condition in the parameters there is an i not in it's place
    }

    /*cover the string number into integar*/
    if (isPort != -1)
    {
        intPort = atoi(newPort);
        if (intPort == 0)
        {
            Nfree(path);
            Nfree(newHost);
            Nfree(newPort);

            Nfree(parameter);
           

            Nfree(buf);
            Nfree(txt);
            UsageError();
        }
    }
    int rc;
    int sockfd;
    char *rbuf = (char *)malloc(BUFLEN + 1);
    for (int x = 0; x < BUFLEN + 1; x++)
    {
        rbuf[x] = '\0';
    }
    //  char rbuf[BUFLEN];
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        Nfree(path);
        Nfree(newHost);
        Nfree(newPort);

        Nfree(parameter);
        Nfree(rbuf);

        Nfree(buf);
        Nfree(txt);
        error("socket failed");
    }

    // connect to server
    if (newHost == NULL)
    {
        Nfree(path);
        Nfree(newHost);
        Nfree(newPort);

        Nfree(parameter);
        Nfree(rbuf);

        Nfree(buf);
        Nfree(txt);
        UsageError();
    }
    server = gethostbyname(newHost);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        Nfree(path);
        Nfree(newHost);
        Nfree(newPort);

        Nfree(parameter);
        Nfree(rbuf);

        Nfree(buf);
        Nfree(txt);
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(intPort);
    rc = connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (rc < 0)
    {
        Nfree(path);
        Nfree(newHost);
        Nfree(newPort);

        Nfree(parameter);
        Nfree(rbuf);

        Nfree(buf);
        Nfree(txt);
        error("connect failed:");
    }
    else
    {
        printf("connected\n");
    }
    /*calculate the request byte size*/
    int bufSize = 0;
    if (p == -1) //get request
    {
        bufSize += strlen("GET ");
    }
    else
    {
        bufSize += strlen("POST "); //shof al text
    }
    bufSize += strlen(path);
    if (r != -1)
        bufSize += strlen(parameter);
    bufSize += strlen(" HTTP/1.0\r\nHost: ");
    bufSize += strlen(newHost);
    if (p != -1)
    {
        bufSize += strlen("\r\n");
        bufSize += strlen("Content-length:");
        char *h = (char *)malloc(strlen(txt) + 1);
        sprintf(h, "%ld", strlen(txt)); //d
        bufSize += strlen(h);
        free(h);
    }
    bufSize += strlen("\r\n\r\n");
    if (p != -1)
    {
        bufSize += strlen(txt);
    }

    buf = (char *)malloc(bufSize + 1);
    for (int g = 0; g < bufSize + 1; g++)
    {
        buf[g] = '\0';
    }
    if (buf == NULL)
    {
        Nfree(path);
        Nfree(newHost);
        Nfree(newPort);

        Nfree(parameter);
        Nfree(rbuf);

        Nfree(buf);
        Nfree(txt);
        error("malloc error");
    }
    /*concatinate the request*/
    if (p == -1)
    {
        strcat(buf, "GET ");
    }
    else
    {
        strcat(buf, "POST ");
    }
    strcat(buf, path);
    if (r != -1)
        strcat(buf, parameter);
    strcat(buf, " HTTP/1.0\r\nHost: ");
    strcat(buf, newHost);

    if (p != -1)
    {
        strcat(buf, "\r\n");
        strcat(buf, "Content-length:");
        char *h = (char *)malloc(strlen(txt) + 1); //////////////here i changed something
        for (int g = 0; g < strlen(txt) + 1; g++)
        {
            h[g] = '\0';
        }
        //char h[strlen(txt)];
        sprintf(h, "%ld", strlen(txt)); //d
        strcat(buf, h);
        free(h);
    }
    strcat(buf, "\r\n\r\n");
    if (p != -1)
    {
        strcat(buf, txt);
    }

    printf("HTTP request =\n%s\nLEN = %ld\n", buf, strlen(buf)); //d

    write(sockfd, buf, strlen(buf) + 1); //send the massage to the server -> socket
    int responseLength = 0;

    do
    {
        rc = read(sockfd, rbuf, BUFLEN);
        if (rc > 0)
        {
            printf("%s\n", rbuf);
            rbuf[rc] = '\0';
            responseLength += strlen(rbuf);
        }
        else if (rc == 0)
        {
            printf("\n Total received response bytes: %d\n", responseLength);
        }
        else
        {
            Nfree(path);
            Nfree(newHost);
            Nfree(newPort);

            Nfree(parameter);
            Nfree(rbuf);

            Nfree(buf);
            Nfree(txt);
            error("read() failed");
        }

    } while (rc > 0);

    close(sockfd);
    Nfree(path);
    Nfree(newHost);
    Nfree(newPort);

    Nfree(parameter);
    Nfree(rbuf);

    Nfree(buf);
    Nfree(txt);
}

int main(int argc, char *argv[])
{
    argcError(argc);
    request(argc, argv);
    return EXIT_SUCCESS;
}
