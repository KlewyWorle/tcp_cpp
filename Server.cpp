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
    std::vector<std::string> messages;
public:
    Server(int port, std::string)
    {
        this->port = port;
    }
    void init()
    {
        server = socket(AF_INET, SOCK_STREAM, 0);
        if (server == -1)
        {
            perror("Error socket");
            exit(EXIT_FAILURE);
        }
        
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
            int32_t size = htonl(ss.str().size());
            std::cout << ntohl(size) << std::endl;
            //std::cout << size << std::endl;
            write(fd, &size, sizeof(size));
            write(fd, ss.str().c_str(), ss.str().size());

            file.close();
        }
    }
    void listen()
    {
        while(true)
        {
            //printf("new\n");
            socklen_t adrlen = sizeof(adr);

            int client_fd = accept(server, (sockaddr*)&adr, &adrlen);
            
            if (client_fd == -1)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }
            //printf("accepted");
            char buf[1024];

            send_file(client_fd);


            //std::cout << buf << std::endl;

            close(client_fd);
        }
    }

};



int main()
{
    Server serv(1469, "none");
    serv.init();
    serv.listen();


}