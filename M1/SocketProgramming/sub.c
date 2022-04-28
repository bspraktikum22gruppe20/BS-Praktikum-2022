
#include "sub.h"
#include <sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include <signal.h>


int read_in(int socket_client, char *buf, int len) {
    char *s = buf;
    int slen = len;
    int c = recv(socket_client, s, slen, 0);

    //Keep reading until there are no more character or you reach  \n
    while ((c > 0) && (s[c - 1] != '\n')) {
        s += c;
        slen -= c;
        c = recv(socket_client, s, slen, 0);
    }
    if (c < 0) return c;
    else if (c == 0) buf[0] = '\0';//Nothing read. send back an empty string
    else s[c - 1] = '\0'; //replace the \r character with \0
    return len - slen;
}

void error(char *msg) {
    fprintf(stderr, "%s:%s\n", msg, strerror(errno));
    exit(1);
}

int open_server_socket() {
    /***
    * PF_INET : protocol family for TCP/IP protocol
    * SOCK_STREAM : Stream sockets(TCP): reliable byte-stream service
    * protocol : socket protocol
    */
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) error("Can't opent socket");
    return s;
}

int bind_to_port(int socket, int port) {
    //address(ip,port) initializing
    //bind need two things: 1- socket descriptor 2-address
    //------------------address initializing
    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET; //Internet protocol (AF_INET)
    server_addr.sin_port = htons(port); //address port 16 bit
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Internet address 32bit , INADDR_ANY = any incoming interface
    //--------------------
    int reuse = 1;//reuse the port
    //This code makes the socket reuse the port,
    // That means you can stop and restart the server and there will be
    // no errors when you bind the port a second time
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int)) == -1)
        error("Can't set the reuse option on the socket");


    int c = bind(socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (c == -1) error("Cannot bind to socket");
}

//send a string to a client
int say(int client_socket, char *s) {
    int result = send(client_socket, s,  strlen(s),0);
    //Don't call error() if there is a problem you won't want to stop the server
    //if there is just a problem with one client
    if (result == -1)
        fprintf(stderr, "%s:%s\n", "Error talking to client", strerror(errno));
    return result;

}

