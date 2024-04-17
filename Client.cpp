#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<vector>
#include<fstream>


class Client
{
private:
    int port;
    sockaddr_in adr{};
    int server;
    std::string msg;

    
public:
    Client(int port, std::string msg) : msg(msg)
    {
        this->port = port;
    }
    int32_t getFileSize(int server)
    {
        int32_t SIZE_BUFFER;
        read(server, &SIZE_BUFFER, sizeof(SIZE_BUFFER));

        return ntohl(SIZE_BUFFER); //Getting file size to make a buffer sized of and converting from htonl
    }
    void getFileContents(int server, char *buf, int32_t SIZE_BUFFER)
    {
        size_t nread = read(server, buf, SIZE_BUFFER); //Reading file (TODO: Error handling)
    }
    void connect()
    {
        server = socket(AF_INET, SOCK_STREAM, 0); //Creating a server socket
        if (server == -1)
        {
            perror("Socket error");
            exit(EXIT_FAILURE);
        }
        
        //Setting up sockaddr_in
        adr.sin_port = htons(port);
        adr.sin_family = AF_INET; 

        int net = inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr); // Putting ip-address into sockaddr_in
        if(net == -1)
        {
            perror("Net error");
            exit(EXIT_FAILURE);
        }

        int con = ::connect(server, (struct sockaddr*)&adr, sizeof(adr)); //Connecting to the server
        if(con == -1)
        {
            perror("Connection errro");
            exit(EXIT_FAILURE);
        }
        
        int32_t SIZE_BUFFER = getFileSize(server); 
        std::cout << SIZE_BUFFER << std::endl;
        char buf[SIZE_BUFFER];
        getFileContents(server, buf, SIZE_BUFFER); //Getting file contents into buf
        buf[SIZE_BUFFER] = '\0'; // Null terminating this motherfucker so it won't break stuff

        std::ofstream file;
        file.open("fuck2.txt"); //Writing data that we got into a file (TODO: Move to a separate function)
        if(file.is_open())
        {
            file << buf;
            file.flush();
            file.close();
        }
        close(server); //Closing the socket
    }
};
int main(int argc, char *argv[])
{

    Client client(1461, "none");
    client.connect();

}