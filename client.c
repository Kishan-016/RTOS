#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
#include <signal.h>

void sigintHandler(int sig_num) 
{ 
    printf("Are you sure want to exit(y/n)");
    if(getchar()=='y') exit(1);
} 

void *recvmg(void *sock)
{
	int rcv_sock = *((int *)sock);
	char msg[2000],put_msg[2000];
    int msgtype;
	int len;
	while((len = recv(rcv_sock,msg,sizeof(msg),0))>0) {
        strcpy(put_msg,msg);
        recv(rcv_sock,&msgtype,sizeof(msgtype),0);
        recv(rcv_sock,msg,sizeof(msg),0);
        if( ntohl(msgtype) == 1){
            printf("%s: %s\n",msg,put_msg);}
        else {
            printf("Hey you have a direct message from %s: %s\n", msg, put_msg);
        }
	}
}

int main(int argc, char const *argv[]) 
{ 
	int sock = 0;
	pthread_t sendt,recvt;
	struct sockaddr_in serv_addr; 
	char msg[2000] ={0};  
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(atoi(argv[1])); 
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, argv[2], &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}
    else
	    printf("You are Online\n");

    char name[25],recipient_name[25];
    strcpy(name,argv[3]);
    int option;
    send(sock ,name, sizeof(name),0);

	signal(SIGINT, sigintHandler); 
	pthread_create(&recvt,NULL,recvmg,&sock);
	while(1){

        printf("Enter the message to be sent: \n");
	    fgets(msg, sizeof(msg), stdin); 
	    send(sock ,msg, sizeof(msg),0);
	    
        printf("Enter '1' for group chat and '0' for direct message: ");
        scanf("%d%*c",&option);
        option = htonl(option);
        //printf("%d",ntohl(option));
        send(sock ,&option, sizeof(option),0);

        if(ntohl(option) == 0)
        {   
            printf("Please enter the recipient name\n");
            scanf("%[^\n]%*c", recipient_name);
            send(sock ,recipient_name, sizeof(recipient_name),0);
        }
    }
	pthread_join(recvt,NULL);
	return 0; 
} 
