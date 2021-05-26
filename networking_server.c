// Include header files
#include "header.h"

int main(int argc, char *argv[])
{

    // decalre variables
    struct sockaddr_in hint;
    struct sockaddr_in client;
    char *message;
    char *buffer[BUFF_MAX];

	if(argc > 2)
    {
		printf("Too many arguments");
		exit(1);
	}
    else if(argc < 2)
    {
        printf("Too few arguments");
		exit(1);
    }

    //	Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Unable to create socket");
        return 1;
    }

    // Bind the ip address and port to a socket
    int port =  atoi(argv[1]);
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
 
    int bindSocket = bind(sock, (struct sockaddr*)&hint, sizeof(hint));

    if(bindSocket < 0)
    {
        printf("Failed to Bind");
        
        //	Close the socket
        close(sock);
        return 0;
   
    }

    // Tell Winsock the socket is for listening
    int listenConn = listen(sock, SOMAXCONN);

    if(listenConn < 0)
    {
        printf("Failed to Listen");
        
        //	Close the socket
        close(sock);
        return 0;
   
    }
    
    // Wait for a connection
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(sock, (struct sockaddr*)&client, &clientSize);
 
    if(clientSocket < 0)
    {
        printf("Failed to Accept Connections");
        
        //	Close the socket
        close(sock);
        return 0;

    }

    int receiveMessage = read(clientSocket, buffer, BUFF_MAX);

    if(receiveMessage < 0)
    {
        printf("Failed to Receive Message");
        
        //	Close the socket
        close(sock);
        return 0;

    }

    //Prepare confirmation message
    message = "OK GOT IT";

     // Send the message
    int sendMessage = send(clientSocket, message, strlen(message), 0);

    if(sendMessage < 0)
    {
        printf("Failed to Send");
        
        //	Close the socket
        close(sock);
        return 0;
   
    }

    //	Close the socket
    close(sock);
    return 0;

}