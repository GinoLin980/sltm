#ifndef NODE_H
#define NODE_H

#include "event.h"

typedef struct Node {
  Event data;
  struct Node *next;
} Node;

Node *new_node(const Event *event);
int insert_node(Node **head, const Event *event);
int delete_node(Node **head, const char *id);
void free_list(Node **head);
void print_list(const Node *head);

#endif // !NODE_H
