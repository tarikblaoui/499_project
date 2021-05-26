// Include header files
#include "header.h"

int main(int argc, char *argv[])
{

    // decalre variables
    struct sockaddr_in hint;
    char *message;
    char *buffer[BUFF_MAX];

	if(argc > 3)
    {
		printf("Too many arguments");
		exit(1);
	}
    else if(argc < 3)
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

    //	Create a hint structure for the server we're connecting with
    int port =  atoi(argv[1]);
    char *ipAddress = "127.0.0.1";

    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, (const char *)ipAddress, &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (struct sockaddr*)&hint, sizeof(hint));
    if (connectRes < 0 )
    {
        printf("Failed to Connect");
        return 1;
    }

    // Get the message from teminal argument
    message = argv[2];

    // Send the message
    int sendMessage = send(sock, message, strlen(message), 0);

    if(sendMessage < 0)
    {
        printf("Failed to Send");
        
        //	Close the socket
        close(sock);
        return 0;
   
    }

    int readMessage = read(sock, buffer, BUFF_MAX);

    if(readMessage < 0)
    {
        printf("Failed to Receive");
        
        //	Close the socket
        close(sock);
        return 0;
   
    }

    printf("\nServer: %s \n", buffer);
    
    //	Close the socket
    close(sock);
    return 0;

}