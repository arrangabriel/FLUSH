#include "list.h"

List *List_create()
{
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

List *List_push(List *list, Command *cmd)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->cmd = cmd;
    node->next = NULL;

    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
    return list;
}

List *List_remove(List *list, Command *cmd)
{
    Node *node = list->head;
    Node *prev = NULL;
    while (node != NULL)
    {
        if (node->cmd == cmd)
        {
            if (prev == NULL)
            {
                list->head = node->next;
                if (list->head == NULL)
                    list->tail = NULL;
            }
            else
            {
                prev->next = node->next;
                if (node->next == NULL)
                    list->tail = prev;
            }
            // Maybe free the command?
            command_del(cmd);
            free(node);
            return list;
        }
        prev = node;
        node = node->next;
    }
    return list;
}

int List_del(List *list)
{
    Node *node = list->head;
    while (node != NULL)
    {
        list->head = node->next;
        command_del(node->cmd);
        free(node);
        node = list->head;
    }
    free(list);
    return 0;
}