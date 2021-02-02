
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *text)
{
    perror(text);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, new_sockfd, port_num;
     socklen_t client_len;
     char buff[255];
     struct sockaddr_in server_addr, client_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("error opening in socket");
     bzero((char *) &server_addr, sizeof(server_addr));
     port_num = atoi(argv[1]);
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_port = htons(port_num);
     if (bind(sockfd, (struct sockaddr *) &server_addr,
              sizeof(server_addr)) < 0) 
              error("error on the binding");
     listen(sockfd,5);
     client_len = sizeof(client_addr);
     new_sockfd = accept(sockfd, 
                 (struct sockaddr *) &client_addr, 
                 &client_len);
     if (new_sockfd < 0) 
          error("error on the accept");
     while(1)
     {
           bzero(buff,255);
           n = read(new_sockfd,buff,255);
           if (n < 0) error("error reading from the socket");
           printf("client: %s\n",buff);
          bzero(buff,255);
          fgets(buff,255,stdin);
          n = write(new_sockfd,buff,strlen(buff));
           if (n < 0) error("error writing to the socket");
           int i=strncmp("See You" , buff, 7);
           if(i == 0)
               break;
     }
     close(new_sockfd);
     close(sockfd);
     return 0; 
}
