#ifndef ____PARSING___H___
#define ____PARSING___H___

#include <stddef.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
int get_line(char *prmpt, char *buff, size_t sz);
int parse_line(char *line, size_t sz, char ***argsm, unsigned int *argc);

#endif