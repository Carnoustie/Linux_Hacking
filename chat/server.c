#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int static const CLIENTSPACE = 10;
char buff[30];
char replyBuff[30];

int main(int argc, char const * argv[]){
    int serverPort = 5000;
    char server_Address_String[] = "192.168.0.23";

    struct sockaddr_in client_Address_Struct;
    struct sockaddr_in server_Address_Struct;
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket!=-1) {
        printf("\n\nSuccessfully created socket for server. Its socket number is %d\n\n", serverSocket); 
    }
    else if (serverSocket==-1) {
        printf("\n\nFailed when creating a socket for server\n\n"); 
    }

    server_Address_Struct.sin_family = AF_INET;
    server_Address_Struct.sin_port = serverPort;
    server_Address_Struct.sin_addr.s_addr = INADDR_ANY;

    int bindResult = bind(serverSocket, (struct sockaddr*) &server_Address_Struct, sizeof(server_Address_Struct));
    if (bindResult==0) {
        printf("\n\nSuccessfully bound the server's socket to its address struct.\n\n"); 
    }
    else if (serverSocket==-1) {
        printf("\n\nFailed at binding socket to address structure\n\n"); 
    }

    int listenResult = listen(serverSocket, CLIENTSPACE);
    if (listenResult==0) {
        printf("\n\nSuccessfully changed the state of the server's socket to listen.\n\n"); 
    }
    else if (listenResult==-1) {
        printf("\n\nSuccessfully changed the state of the server's socket to listen.\n\n"); 
    }

    int acceptResult;
    int recvResult;
    socklen_t Length_client_addr_struct = sizeof(client_Address_Struct);
    acceptResult = accept(serverSocket, (struct sockaddr*) &client_Address_Struct, &Length_client_addr_struct);
    if (acceptResult>1) {
    while(1){
        //printf("\n\nEntered loop\n\n");
           // printf("\n\nsuccessful connection to client\n\n"); 
            recvResult = recv(acceptResult, buff, sizeof(buff), 0);
            if(recvResult>0){
                printf("\n\n%s\n\n", buff);
                buff[recvResult] = '\0';
                //scanf("%29s", replyBuff);
                fgets(replyBuff, sizeof(replyBuff), stdin);
                send(acceptResult, replyBuff, sizeof(replyBuff), 0);
                //printf("\n\nHit here\n\n");
            }
        }
    }else if(acceptResult==-1)
        printf("\n\nFailed connection to client\n\n"); 
}
