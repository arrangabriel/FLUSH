#ifndef __LL_H__
#define __LL_H__

#include "command.h"

typedef struct Node
{
    Command *cmd;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    Node *tail;
    int len;
} List;

/**
 * @brief Creates a new empty list struct.
 *
 * @return A pointer to the new list.
 */
List *list_init();

/**
 * @brief Delete list, !warning! frees commands
 *
 * @param list List to delete
 * @return Status
 */
int list_del(List *list);

/**
 * @brief Add a command to the end of the list.
 *
 * @param list List to add to
 * @param cmd Command to add
 *
 * @return Status
 */
int *list_push(List *list, Command *cmd);

/**
 * @brief Remove a command from the list.
 *
 * @param list List to remove from
 * @param cmd Command to remove
 *
 * @return Status
 */
int *list_remove(List *list, Command *cmd);

/**
 * @brief Get length of list
 *
 * @param list List to get length of
 * @return int Length of list
 */
int list_length(List *list);

#endif