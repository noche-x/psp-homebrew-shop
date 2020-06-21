#include <iostream>     //  cout, endl
#include <fstream>      //  ofstream
#include <iomanip>      //  setw

#include <signal.h>      //  signal
#include <sys/socket.h> //  network stuff
#include <arpa/inet.h>  //  network stuff
#include <unistd.h>     //  network stuff
#include <pthread.h>    //  threads

#include "json.hpp"     //  json lib
#include "stardust_networking.hpp"
#include "definitions.hpp"  //  log and stuff

void exit_program(int sig);

void *handle_connection(void *p_client_socket);

int main() {
    signal(SIGINT, exit_program);  
    sockets_to_close.clear();
    
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000] = {0};

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    IF((socket_desc == -1), "socket created", "socket creation failed...", 1);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(35700);

    //Bind
    IF(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0, "bind done", "bind failed...", 1);

    //Listen
    IF(listen(socket_desc, 100) != 0, "listening to socket", "listen failed...", 1);

    bool running = true;

    while (running)
    {
        //INFO("waiting for connections...");
        //c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        //IF(accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c) < 0, "connection accepted", "accept failed", 1);

        // //accept connection from an incoming client
        // client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        // if (client_sock < 0)
        // {
        //     perror("accept failed");
        //     return 1;
        // }
        // puts("Connection accepted");

        //Accept and incoming connection
        INFO("waiting for connections...");
        c = sizeof(struct sockaddr_in);

        //accept connection from an incoming client
        // client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        // if (client_sock < 0)
        // {
        //     perror("accept failed");
        //     return 1;
        // }
        // puts("Connection accepted");
        auto client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
        IF(client_sock < 0, "connection accepted", "accept failed", 1);

        //sockets_to_close.push_back(client_sock);

        pthread_t thread;
        int *pclient = (int*)malloc(sizeof(int));
        *pclient = client_sock;
        pthread_create(&thread, NULL, handle_connection, pclient);
        
        //Receive a message from client
        //int recv_a = 0;
        //if (recv_a = recv(client_sock, client_message, 2000, 0) > 0)
        //   write(client_sock, client_message, strlen(client_message)); //Send the message back to client
    }

    LOG("disconnecting clients and closing server");
    return 0;  




    nlohmann::json j;
    j["pi"] = 3.141;
    
    std::ofstream o("pretty.json");
    o << std::setw(4) << j << std::endl;
    
}

void *handle_connection(void *p_client_socket)
{
    int client_sock = *(int*)p_client_socket;
    free(p_client_socket);

    sockets_to_close.push_back(client_sock);

    using namespace Stardust::Network;
    PacketIn pIn;

    std::vector<byte> len;
    byte newByte;
    IF(recv(client_sock, &newByte, 1, 0) == -1, "recieved first part", "recieve first failed", NULL);

    while (newByte & 128)
    {
        len.push_back(newByte);
        IF(recv(client_sock, &newByte, 1, 0) == -1, "", "recieve failed", NULL);
    }
    len.push_back(newByte);

    //We now have the length stored in len
    int length = decodeVarInt(len);

    int totalTaken = 0;

    byte *b = new byte[length];
    totalTaken += recv(client_sock, b, length, 0);

    for (int i = 0; i < length; i++)
    {
        pIn.bytes.push_back(b[i]);
    }

    pIn.pos = 0;

    printf("[+] a: ");
    for (int i = 0; i < pIn.bytes.size(); i++)
    {
        printf("%x ", pIn.bytes[i]);
    }
    std::cout << std::endl;
    printf("[+] a: ");
    for (int i = 0; i < pIn.bytes.size(); i++)
    {
        printf("%c ", pIn.bytes[i]);
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    pIn.ID = decodeShort(pIn);

    LOG("packet id: " << pIn.ID);
    LOG("packet bytes: " << std::string(pIn.bytes.begin(), pIn.bytes.end()));

    
    PacketOut *new_packet = new PacketOut;

    switch (pIn.ID) {
        case PacketIDS::NONE:
            break;
        case PacketIDS::GET_APPS:
        {
            new_packet->ID = PacketIDS::SEND_APPS;
            
            std::ifstream apps_file("apps.json");
            if (apps_file.is_open())
            {
                std::string apps_list = "";

                apps_file.seekg(0, std::ios::end);
                apps_list.reserve(apps_file.tellg());
                apps_file.seekg(0, std::ios::beg);

                apps_list.assign((std::istreambuf_iterator<char>(apps_file)),
                           std::istreambuf_iterator<char>());

                encodeString(apps_list, *new_packet);
            }
            else {
                ERROR("unable to open file apps.json");
            }
            
            
            break;
        }
        case PacketIDS::GET_FILE:
        {
            new_packet->ID = PacketIDS::SEND_FILE;

            std::string file_name = decodeString(pIn);
            LOG("file name: " << file_name);
            break;
        }
    };
    
    std::cout << std::endl;

    
    std::vector<byte> endByteBuffer;

    int packetLength = new_packet->bytes.size() + 2;

    //Header
    encodeVarInt(packetLength, endByteBuffer);
    encodeShort(new_packet->ID, endByteBuffer);

    //Add body
    for (int x =0; x < new_packet->bytes.size(); x++)
    {
        endByteBuffer.push_back(new_packet->bytes[x]);
    }

    printf("[+] hashedChars: ");
    for (int i = 0; i < endByteBuffer.size(); i++)
    {
        printf("%x ", endByteBuffer[i]);
    }
    std::cout << std::endl;

    //Send over socket
    //if (send(client_sock, endByteBuffer.data(), endByteBuffer.size() + 1, 0) > 0)
    //  std::cout << "yes" << std::endl;
    IF(send(client_sock, endByteBuffer.data(), endByteBuffer.size(), 0) == -1, "sent the packet", "send failed", NULL);

    close(client_sock);
    return NULL;
}

void exit_program(int sig) {
    LOG("Caught SIG_TERM");

    for (auto &&i : sockets_to_close)
    {
        close(i);
    }
    
    exit(sig);
};