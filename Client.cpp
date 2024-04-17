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

        return htonl(SIZE_BUFFER);
    }
    void getFileContents(int server, char *buf, int32_t SIZE_BUFFER)
    {
        
        size_t nread = read(server, buf, SIZE_BUFFER);
    }
    void connect()
    {
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == -1)
        {
            perror("Socket error");
            exit(EXIT_FAILURE);
        }
        adr.sin_port = htons(port);

        adr.sin_family = AF_INET;

        int net = inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
        if(net == -1)
        {
            perror("Net error");
            exit(EXIT_FAILURE);
        }

        int con = ::connect(server, (struct sockaddr*)&adr, sizeof(adr));
        
        //write(server, msg.c_str(), msg.size());
        //write(fd, &size, sizeof(size));
        

        int32_t SIZE_BUFFER = getFileSize(server);
        std::cout << SIZE_BUFFER << std::endl;

        char buf[SIZE_BUFFER];

        getFileContents(server, buf, SIZE_BUFFER);

        
        buf[SIZE_BUFFER] = '\0';
        //std::cout << buf << std::endl;
        
        std::ofstream file;
        file.open("fuck2.txt");
        if(file.is_open())
        {
            file << buf;
            file.flush();
            file.close();
        }
        
        
        close(server);
    }

};



int main(int argc, char *argv[])
{

    Client client(1469, "none");
    client.connect();

}