//
// Created by abhei on 06.04.2022.
//

#ifndef SUB_H
#define SUB_H

//reading from the client
int read_in(int, char *, int);

int open_server_socket();

int bind_to_port(int, int);

int say(int, char *);

void error(char *);

#endif
