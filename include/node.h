#ifndef NODE_H
#define NODE_H

#include "event.h"

#define NODE_SUCCESS 0
#define NODE_ERROR -1
#define NODE_ERROR_NOT_FOUND -2
#define NODE_ERROR_NO_MEMORY -3
#define NODE_ERROR_BAD_PARAM -4

typedef struct Node {
  Event data;
  struct Node *next;
} Node;

Node *new_node(const Event *event);
int insert_node(Node **head, const Event *event);
int delete_node(Node **head, const char *id);
Node *find_node_by_id(Node **head, const char *id);
void range_and_print_node(Node **head, char *date1, char *date2);
void find_and_print_node(Node **head, const char *keyword);
void free_list(Node **head);
void print_list(const Node *head);

#endif // !NODE_H
