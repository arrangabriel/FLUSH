#include "parsing.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
    char *prmpt = ">";
    char buff[1024];
    int status;
    while ((status = getLine(prmpt, buff, sizeof(buff))) != NO_INPUT) {
        if (status == TOO_LONG) {
            printf("Too long\n");
        } else {
            printf("%s\n", buff);
        }
    }
    printf("exit\n");
    return 0;
}

