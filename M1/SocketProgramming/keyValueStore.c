

#include "keyValueStore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct entry {
    char key[256];
    char value[256];
} entry;

struct entry data[300];
int i = 0;


int put(char *key, char *value) {
    strcpy(data[i].key, key);
    strcpy(data[i].value, value);
    i++;
    return 0;
}

int get(char *key, char *res) {
    char str[30];
    strcpy(str,key);
    int len = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < len; i++) {
        if (strcmp(data[i].key ,str)) {
            strcpy(res, data[i].value);
            break;
        }
    }
    if (i==len) strcpy(res,"value not found");
    return 1;
}

int del(char *key) {

    return 0;
}

