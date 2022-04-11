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

List *list_init();

/**
 * @brief delete list, !warning! frees commands
 * 
 * @param list - list to delete
 * @return int - status
 */
int list_del(List *list);

List *list_push(List *list, Command *cmd);

List *list_remove(List *list, Command *cmd);

#endif