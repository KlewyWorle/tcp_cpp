#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<vector>
#include<ranges>
#include<sstream>
#include<fstream>


class Server
{
private:
    int port;
    sockaddr_in adr{};
    int server;
    
public:
    Server(int port, std::string)
    {
        this->port = port;
    }
    ~Server()
    {
        printf("Destructor\n");
        close(server);
    }
    void init()
    {
        server = socket(AF_INET, SOCK_STREAM, 0); //Creating a socket
        if (server == -1)
        {
            perror("Error socket");
            exit(EXIT_FAILURE);
        }
        
        //Setting app sockaddr
        adr.sin_port = htons(port);
        adr.sin_family = AF_INET;

        if (bind(server, (sockaddr*)&adr, sizeof(adr)) == -1)
        {
            perror("Bind error");
            exit(EXIT_FAILURE);
        } 
        if(::listen(server, 5) == -1)
        {
            perror("Listen error");
            exit(EXIT_FAILURE);
        }
    }
    void send_file(int fd)
    {
        std::ifstream file;
        std::stringstream ss;
        file.open("fuck.txt");
        if(file.is_open())
        {
            file >> ss.rdbuf();
            int32_t size = htonl(ss.str().size()); //Converting value so it will be "cross-platform"

            /*
                htonl - to 
                ntohl - from
            */
            std::cout << ntohl(size) << std::endl;
            //std::cout << size << std::endl;
            write(fd, &size, sizeof(size)); //Writing data to client
            /*
                TODO: Use send/recv instead of write/read===============================================
            */
            write(fd, ss.str().c_str(), ss.str().size());
            file.close();
        }
    }
    void listen()
    {
        while(true) //infinite loop for infinite listening to server
        {
            //printf("new\n");
            socklen_t adrlen = sizeof(adr);
            int client_fd = accept(server, (sockaddr*)&adr, &adrlen); //Connecting server to client
            if (client_fd == -1)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            send_file(client_fd); 


            close(client_fd);
        }
    }
};
int main()
{
    Server serv(1461, "none");
    serv.init();
    serv.listen();
}