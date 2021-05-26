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
#include <stack> 
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;
//Client side
string send_data(int port_num, string message)
{
    usleep(100000);
    //we need 2 things: ip address and port number, in that order

    const char *serverIp = "localhost"; int port = port_num; 
    //create a message buffer 
    char msg[1500]; 
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl;
    }

    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    while(1)
    {
        cout << ">";
        string data;
        data = message;
        //getline(cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
    
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }
        break;
    }
    gettimeofday(&end1, NULL);
    close(clientSd);
   
    return msg; 
}


int str_to_int (string str) {
            stringstream degree(str);
            int conv = 0;
            degree >> conv;
            return conv;
        }



int get_port(int id){
    if (id == 1){
        return 8081;
    }else if (id == 2) {
        return 8082;
    }else if (id == 3){
        return 8083;
    }

    return 0;
}

string int_to_str (int &block) {

            string str_block = to_string(block);


            switch(str_block.length()){
                case (1):
                    str_block = "0" + str_block; 
                    break;
            }

            return str_block;
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

 vector<int> find_empty_block (int start_block){
            int start_position = start_block * 204;
            vector<int> return_vec;
            std::stack<int> block_stack;
            std::stack<int> server_stack;
            block_stack.push(start_block);
            server_stack.push(0);
            string next_data;

            string data =  read_from_file(start_position,204,"disks/disk0.txt"); 
          
            int next_block = 0;
            int next_start_position = 0;
            int next_server = 0;


           if (data.back() == ' '){
        
                    return_vec.push_back(block_stack.top());
                    return_vec.push_back(server_stack.top());
                    return return_vec;
            }
            
            while (true){

                server_stack.push(next_server);
                block_stack.push(next_block);
                if (next_server == 0){
                     data =  read_from_file(next_block * 204,204,"disks/disk0.txt");
          
                }
                else {
                    int next_port = get_port(next_server);
         
              
                     data = send_data(next_port,"3:" + to_string(next_server) + int_to_str(next_block));
                }
                
     

             


                if (data.back() == ' '){
                    
                    return_vec.push_back(block_stack.top());
                    return_vec.push_back(server_stack.top());

                    break;}
                next_start_position = next_block * 204;
                start_position = next_start_position;
                next_block = str_to_int(data.substr(1,3));
                next_server = str_to_int(data.substr(0,1));
                //data = next_data;
            }

            
            return return_vec;
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



string data_padding(string &data){
    string padded_data;
    while (data.length() < 200) {
        data = data + "|";
    } 
    return data;
}

std::vector<int> write_block(int server_id, int server_block, string data){
    string block_info;
    string data_tbr;
    int next_block;
    int next_server;
    if (server_id == 0){
        block_info = read_from_file(server_block * 204,3,"disks/disk0.txt");
        data_tbr = block_info + data;
        write_to_file(server_block*204,204,data_tbr,"disks/disk0.txt");
   
        next_block = str_to_int(block_info.substr(1));
        next_server = str_to_int(block_info.substr(0,1));
    }
    else{
        block_info = send_data(get_port(server_id), "1:"+ int_to_str(server_block)  + data);
    }
   next_block = str_to_int(block_info.substr(1));
   next_server = str_to_int(block_info.substr(0,1));
   std::vector<int> return_vec;
   return_vec.push_back(next_server);
   return_vec.push_back(next_block);
   return return_vec;
 } 



std::vector<std::string> split(const std::string &str, const std::string &delim)
            {
                std::vector<std::string> tokens;
                size_t prev = 0, pos = 0;
                do
                {
                    pos = str.find(delim, prev);
                    if (pos == std::string::npos)
                        pos = str.length();
                    std::string token = str.substr(prev, pos - prev);
                    if (!token.empty())
                        tokens.push_back(token);
                    prev = pos + delim.length();
                } while (pos < str.length() && prev < str.length());
                return tokens;
 }

std::vector<string> file_exist(string file_name) {

    std::ifstream input( "disks/file_system0.txt" );

    std::vector<string> file_data;

      std::ifstream file("file_systems/file_system0.txt");
      std::string str;
      bool file_exists = false;
      while (std::getline(file, str)) {
        if (file_name == split(str,"|")[2] ) {
            file_exists = true;
            file_data.push_back("Y");
            file_data.push_back(split(str,"|")[0]);
            file_data.push_back(split(str,"|")[1]);
        }
      }

     if(!file_exists){
        file_data.push_back("N");
     }

      return file_data;
}

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}


string read_file(string file_name) {
    string output_data = "";

    if (file_exist(file_name)[0] == "N") {
        return "Error: FILE '" + file_name+ "' NOT FOUND";
    }

    //string file_

    string file_start_server = file_exist(file_name)[1];
    string file_start_block = file_exist(file_name)[2];
    string data;

    while (1){
    int int_start_block = str_to_int(file_start_block);
    int start_bit = int_start_block * 204;
    if (file_start_server == "0") {
        data = read_from_file(start_bit,204,"disks/disk0.txt");
      
    }
    else {
        data = send_data(get_port(str_to_int(file_start_server)),"3:"+file_start_server + file_start_block);
    }

    findAndReplaceAll(data,"|","");
    output_data = output_data + data.substr(3,200);

    file_start_server =  data.substr(0,1);
    file_start_block = data.substr(1,2);

   

    if (*data.rbegin() == ']') {break;};
    }

    output_data = output_data.substr(0, output_data.size()-1);

    return output_data;
}

void creat_new_file(string file_name, string data){

    if (file_exist(file_name)[0] == "Y") {
        cout << "Error: FILE " + file_name + " ALREADY EXISTS" << endl;
        return;
    }
    
 
  
    int next_server;
    int next_block;
    int len_str = data.length();
    int required_blocks = ceil(data.length()/ 200.0);
    int x = 0;
    std::vector<int> start_server = find_empty_block(0);

    next_block = start_server[0];
    next_server = start_server[1];


   
    send_data(get_port(1),"0:"+ to_string(next_server)+  "|" + int_to_str(next_block)+ "|" +file_name);
    send_data(get_port(2),"0:"+ to_string(next_server)+  "|" + int_to_str(next_block)+ "|" +file_name);
    send_data(get_port(3),"0:"+ to_string(next_server)+  "|" + int_to_str(next_block)+ "|" +file_name);
    std::ofstream outfile;
    outfile.open("file_systems/file_system0.txt", std::ios_base::app); // append instead of overwrite
    outfile <<  to_string(next_server)+  "|" + int_to_str(next_block)+ "|" +file_name << endl; 


    while (x < required_blocks ) {
        string data_to_pad = data.substr(x*200,x+200);
        
        string padded_data =  data_padding(data_to_pad);

        

        if (x == required_blocks - 1) {
            padded_data = padded_data + "]";
        }
        else {
            padded_data = padded_data + "[";
        }


        x = x +1;
        std::vector<int> next_write = write_block(next_server,next_block,padded_data);
        next_server =  next_write[0];
        next_block = next_write[1];


       
    }  


}

int main(int argc, char **argv){

    //string server_resp = send_data(8080,"2");

    //string add_entry =  send_data(8080,"0:4|67|file_name.txt");

    //string get_block = send_data(8080,"3:151");
    //cout << get_block << endl;
    //std::vector<int> v =  find_empty_block(0) ;


    string data = "this is a test";
    //string padded = data_padding(data) + "]";
    //cout << data_padding(data) << endl;
    //std::vector<int> next_write = write_block(1,0,padded);

    //creat_new_file("test.txt","dfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsds");

    //std::vector<int> v =  find_empty_block(0);

    //cout << v[0] << endl;
    //cout << v[1] << endl;

    //cout << read_file("test.txt") << endl ;

    //creat_new_file("test.4","This is my test file lets hope it worksdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsdsdfskflsdfkldskfldskfldskflsdfdsfsdfdsfsdfsdfsdfsds");
    //creat_new_file("test.19","This is the data i need to inputThis is the data i need to inputThis is the data i need to inputThis is the data i need to inputThis is the data i need to inputThis is the data i need to inputThis is the data i need to input!!!!! POOOOOOOOP");
    //cout << read_file("test.19") << endl ;
    //cout << read_from_file(1836,204,"disks/disk0.txt") << endl;
    string directive  = argv[1];

    if (directive == "cat") {
        string data = argv[2];
        string file_name = argv[3];
        creat_new_file(file_name,data);
        cout << ">>" << endl;
    }
    else if (directive == "echo") {
        string file_name = argv[2];
        cout << read_file(file_name) << endl ;
    }

    return 0;
}

