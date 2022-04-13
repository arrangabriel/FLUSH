#include "list.h"

List *list_init()
{
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
    return list;
}

List *list_push(List *list, Command *cmd)
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
    (list->len)++;
    return list;
}

List *list_remove(List *list, Command *cmd)
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
            free(node);
            (list->len)--;
            return list;
        }
        prev = node;
        node = node->next;
    }
    return list;
}

int list_del(List *list)
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

int list_length(List *list)
{
    return list->len;
}