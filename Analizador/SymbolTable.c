#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node
{
    bool isFunction;
    char *value;
    char *type;
    char *key; // Key es el id de una variable o funcion
    struct node *next;
};
struct node *head;
struct node *current;

void insert(char *key, char *type)
{
    struct node *link = (struct node *)malloc(sizeof(struct node));
    link->key = key;
    link->type = type;
    link->next = head;
    head = link;
}

void update(char *key, char *value, bool isFunction)
{
    if (head == NULL)
    {
        return;
    }
    struct node *current = head;
    while (strcmp(current->key, key) != 0)
    {
        if (current->next != NULL)
        {
            current = current->next;
        }
        else
        {
            return;
        }
    }
    current->isFunction = isFunction;
    current->value = value;
}

bool find(char *key)
{
    if (head == NULL)
    {
        return false;
    }

    struct node *current = head;
    while (strcmp(current->key, key) != 0)
    {
        if (current->next != NULL)
        {
            current = current->next;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void printTable()
{
    if (head == NULL)
    {
        return;
    }
    printf("Symbol Table\n");
    printf("\n");
    struct node *ptr = head;
    printf("SymbolNumber\tID\t\tType\t\tValue\t\tFunction\n");
    int i = 0;
    while (ptr != NULL)
    {
        printf("%d\t\t%s\t\t%s\t\t%s\t\t%d\n", i++, ptr->key, ptr->type, ptr->value, ptr->isFunction);
        ptr = ptr->next;
    }
}