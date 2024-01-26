#include "fpgaaccessremote.h"

#include <iosfwd>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/router.h>



#include <iostream>
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread


// Example from : https://gist.github.com/GaryMeloney/424e502956f49b2fc58487d9d510ed80

//the thread function
void *connection_handler(void *);

void* start_fpga_server(void *)
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return NULL;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return NULL;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return NULL;
    }

    return NULL;
}


// Not the best pattern, could be written differently in the future
static int sock = 0;


/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    sock = *(int*)socket_desc;

    return nullptr;
}


uint32_t fpga_compute(uint32_t a, uint32_t b, uint32_t c)
{
    char client_message[2000];
    int read_size;
    // Prepare response
    std::stringstream stream;
    stream << "{compute:" << a << "," << b << "," << c << "}\n";

    std::string message = stream.str();
    write(sock, message.data(), strlen(message.data()));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';

        std::cout << "Received from FPGA: " << client_message << std::endl;

        std::string result = client_message;
        auto p = result.find_first_of(":");
        auto valueS = result.substr(p+1);
        uint32_t value = atol(valueS.data());


        return value;
        //clear the message buffer
        memset(client_message, 0, 2000);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;

}

uint32_t fpga_read(uint32_t addr)
{
    char client_message[2000];
    int read_size;
    // Prepare response
    std::stringstream stream;
    stream << "{rd:" << addr << "}\n";

    std::string message = stream.str();
    write(sock, message.data(), strlen(message.data()));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';

        std::cout << "Received from FPGA: " << client_message << std::endl;

        std::string result = client_message;
        auto p = result.find_first_of(":");
        auto valueS = result.substr(p+1);
        uint32_t value = atol(valueS.data());


        return value;
        //clear the message buffer
        memset(client_message, 0, 2000);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;

}

uint32_t fpga_write(uint32_t addr, uint32_t val)
{
    char client_message[2000];
    int read_size;
    // Prepare response
    std::stringstream stream;
    stream << "{wr:" << addr << "," << val << "}\n";

    std::string message = stream.str();
    write(sock, message.data(), strlen(message.data()));

    return 0;

}



FPGAAccess::FPGAAccess()
{

}

FPGAAccess& FPGAAccess::getInstance()
{
    static FPGAAccess access;
    return access;
}

void FPGAAccess::init()
{
    pthread_t fpga_server_thread;
    if (0 != pthread_create(&fpga_server_thread, NULL, start_fpga_server, NULL)) {
        std::cout << "Can not start the FPGA server" << std::endl;
        return;
    }

}

uint32_t FPGAAccess::compute(uint32_t a, uint32_t b, uint32_t c)
{
    return fpga_compute(a, b, c);
}


uint32_t FPGAAccess::getNbCompute()
{
    return fpga_read(6);
}

void FPGAAccess::resetNbCompute()
{
    fpga_write(6, 1);
}
