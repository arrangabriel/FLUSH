#include "parsing.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
    char *prmpt;
    char *buff;
    size_t sz;
    int status;
    while ((status = getLine(prmpt, buff, sz)) != EOF) {
        if (status == NO_INPUT) {
            printf("No input\n");
        } else if (status == TOO_LONG) {
            printf("Too long\n");
        } else {
            printf("%s\n", buff);
        }
    }
    return 0;
}

