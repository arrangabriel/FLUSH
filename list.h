#ifndef __LL_H__
#define __LL_H__

#include "command.h"

// define linked list
typedef struct Node
{
    Command *cmd;
    struct Node *next;
} Node;

// define linked list
typedef struct List
{
    Node *head;
    Node *tail;
} List;

List *List_create();

int List_del(List *list);

List *List_push(List *list, Command *cmd);

List *List_remove(List *list, Command *cmd);

#endif