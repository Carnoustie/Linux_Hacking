#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>


char sendBuff[30];
char recvBuff[30];


int main(int argc, char const * argv[]){

    int serverPort = 5000;
    char server_Address_String[] = "192.168.0.23";

    struct sockaddr_in client_Address_Struct;
    struct sockaddr_in server_Address_Struct;

    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket!=-1)
        printf("\n\nSuccessfully created socket for client. Its socket number is %d\n\n", clientSocket);
    else if(clientSocket==-1)
        printf("\n\nFailed when creating socket for client\n\n");

    server_Address_Struct.sin_family = AF_INET;
    server_Address_Struct.sin_port = serverPort;

    int ptonResult = inet_pton(AF_INET,server_Address_String, &server_Address_Struct.sin_addr.s_addr);

    int connectResult = connect(clientSocket, (struct sockaddr*) &server_Address_Struct, sizeof(server_Address_Struct));
    if(connectResult==0)
        printf("\n\nSuccessfully connected client to server with address %s and port %d\n\n",server_Address_String, serverPort);
    else if(connectResult==-1)
        printf("\n\nFailed when connecting client to server\n\n");

    printf("\n\nLet's chat!\n\n");
    char message[] = "\n\nBonjour!\n\n";
    while(1){
        //printf("\n\nEntered loop\n\n");
        //scanf("%29s", sendBuff);
        fgets(sendBuff, sizeof(sendBuff), stdin);
        int sendResult = send(clientSocket, sendBuff, sizeof(sendBuff), 0);
        int recvResult = recv(clientSocket, recvBuff, sizeof(recvBuff),0);
        printf("\n\n%s\n\n", recvBuff);
    }
}
