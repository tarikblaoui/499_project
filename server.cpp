// By Tarik Blaoui
// Class CSCI 49900
// Porfessor Soumik Dey
// This file is for the group project


#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

//Server side

// ./sever 8080 0


int str_to_int (string str) {
            stringstream degree(str);
            int conv = 0;
            degree >> conv;
            return conv;
        }


void add_entry(string file_descriptor,string information){
     std::ofstream outfile;

      outfile.open("file_systems/file_system" + file_descriptor + ".txt", std::ios_base::app); // append instead of overwrite
      outfile <<  information << endl; 

    }

std::string read_from_file(int start_position,int number_char,string file_name){
            fstream File(file_name, ios::in | ios::out );
            File.seekg(start_position, ios::beg);
            char F[number_char];
            File.read(F, number_char);
            F[number_char] = 0;
            string block_data(F);
            return block_data;
         }

string get_block_data(string file_descriptor,string coordinates) {
    cout << coordinates.substr(1) << endl;
    int int_coordinates = str_to_int(coordinates.substr(1)) * 204;
    cout << int_coordinates << endl;
    string data = read_from_file(int_coordinates,204,"disks/disk" + file_descriptor + ".txt");
    return data;
}

void write_to_file(int start_position, int number_char, string data , string file_name){
            const char * C = data.c_str();
            fstream Disk(file_name, ios::in | ios::out );
            Disk.seekg(start_position, ios::beg);
            char F[number_char];
            Disk.read(F, number_char);
            F[number_char] = 0;
            Disk.seekg(start_position, ios::beg);
            Disk.write(C,number_char);
             }

string write_block(string server_num, int server_block, string data){
    string block_info;
    string data_tbr;
    string next_block;
    string next_server; 

    block_info = read_from_file(server_block * 204,3,"disks/disk" +server_num+  ".txt");
    data_tbr = block_info + data;
    cout << data_tbr << endl;

    write_to_file(server_block*204,204,data_tbr,"disks/disk"+server_num+".txt");
    cout << "data_written" << endl;
    next_block = block_info.substr(1);
    next_server = block_info.substr(0,1);

   return next_server+next_block;
 } 


int main(int argc, char *argv[])
{   

    while (1) {
   
    string server_num = argv[2];
    //for the server, we only need to specify a port number

    //grab the port number
    int port = atoi(argv[1]);
    //buffer to send and receive messages with
    char msg[1500];
     
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }

    while(1)
    {
    cout << "Waiting for a client to connect..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;
    //lets keep track of the session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    //also keep track of the amount of data sent as well
    int bytesRead, bytesWritten = 0;
    
        //receive a message from the client (listen)
    
        cout << "Awaiting client response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        

        //write logic here 



        string data;

        string str_msg(msg);
        string directiv = str_msg.substr(0, 1);
        cout << directiv << endl;
        if (str_to_int(directiv) == 0) {
    
            add_entry(server_num,str_msg.substr(2));
            data = "added_entry:" + str_msg.substr(2);
            
        } 

        else if(str_to_int(directiv) == 1) {
            cout<< "write" << endl;
            cout << str_msg << endl;

            int int_block  = str_to_int(str_msg.substr(2,2));

            data = write_block(server_num, int_block,str_msg.substr(4));
           
        }

         else if(str_to_int(directiv) == 2) {
            cout<< "directive is to read" << endl;
        } 


        else if(str_to_int(directiv) == 3) {
            string block_data = get_block_data(server_num,str_msg.substr(2));
            cout << block_data << endl;
            data = block_data;
        }
        


        // end of logic
        memset(&msg, 0, sizeof(msg)); //clear the buffer

        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        //send the message to client
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);

    }
    //we need to close the socket descriptors after we're all done
    /*
      cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) 
        << " secs" << endl;
    cout << "Connection closed..." << endl;
    gettimeofday(&end1, NULL);
    //close(newSd);
    close(serverSd);
    */
    }
    return 0;   
}

