#include <sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include <signal.h>
#include "sub.h"

int server_socket;

//if someone hits ctrl-c this function will close the socket
void handle_shutdown(int sig) {
    if (server_socket)
        close(server_socket);
    fprintf(stderr, "Bye!\n");
    exit(0);
}


int main() {
    signal(SIGINT, handle_shutdown);
    //=============================================================================
    //                                  Create socket
    //=============================================================================
    server_socket = open_server_socket();
    //=============================================================================
    //                                 Bind to  socket
    //=============================================================================
    bind_to_port(server_socket, 5678);

    //=============================================================================
    //                     listening to incoming connection
    //=============================================================================
    if (listen(server_socket, 10) == -1) error("Can't listen");

    //=============================================================================
    //                     accept new connection
    //=============================================================================
    struct sockaddr_storage client_addr;// store details about the client who's just connected
    unsigned int address_size = sizeof(client_addr);
    puts("Waiting for Connection....\n");
    char buf[1024];

    while (1) {
        //listen for a connection
        int new_client_socket = accept(server_socket, (struct sockaddr *) &client_addr,
                                       &address_size);//creates descriptor for a new socket
        if (new_client_socket == -1) error("Can't open secondary socket");

        //Sending data to the client
        if (say(new_client_socket,
                "Internet Knock Protocol Server\r\nVersion 1.0\r\nKnock!Knock\n") != -1) {
            //Read data from the client
            read_in(new_client_socket, buf, sizeof(buf));

            if (strncasecmp("Hi", buf, 2))
                say(new_client_socket, "You should say 'Hi?' "); //Checking the user's answer
            else {
                if (say(new_client_socket, "Oscar\r\n") != -1) {
                    read_in(new_client_socket, buf, sizeof(buf));
                    if (strncasecmp("Oscar who?", buf, 10))
                        say(new_client_socket, "You should say ‘Oscar who?’!\r\n");
                    else
                        say(new_client_socket, "Oscar silly question, you get a silly answer\r\n");
                }

            }


        }
        close(new_client_socket);//close secondary socket we used for conversation

    }

    return 0;
}


