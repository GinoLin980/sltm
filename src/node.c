#include "../include/node.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

Node *new_node(const Event *event) {
  Node *n = malloc(sizeof(Node));
  if (n == NULL) {
    return NULL;
  }

  n->data.id = strdup(event->id);
  n->data.date = strdup(event->date);
  n->data.vehicle = strdup(event->vehicle);
  n->data.mission = strdup(event->mission);
  n->data.site = strdup(event->site);
  n->data.status = event->status; // Value type - OK to copy
  n->next = NULL;

  // check if any strdup failed
  if (!n->data.id || !n->data.date || !n->data.vehicle || !n->data.mission ||
      !n->data.site) {
    free_event(&n->data);
    free(n);
    return NULL;
  }

  return n;
}

int insert_node(Node **head, const Event *event) {
  // validate input, addresses aren't passed
  if (head == NULL || event == NULL) {
    return NODE_ERROR_BAD_PARAM;
  }

  Node *new = new_node(event);
  if (new == NULL) {
    return NODE_ERROR_NO_MEMORY;
  }

  // the list is empty(Node* is NULL) or `new.date` is smaller than first item
  // in list
  if (*head == NULL || compare_date((*head)->data.date, new->data.date) == 1) {
    new->next = *head; // point `new.next` to original `head` or `NULL`
    *head = new;       // let `new` being NEW `head`
    return NODE_SUCCESS;
  }

  // copy of `*head`
  Node *current = *head;

  while (current->next != NULL) {
    // new's date is smaller than current's next's date
    // therefore we want to insert between current and current's next
    if (compare_date(current->next->data.date, new->data.date) == 1) {
      // found the current is where we want to insert;
      break;
    }
    // switch to next node
    current = current->next;
  }

  // current -> next | new -> NULL
  // current -> next | new -> next
  new->next = current->next;
  // current -> next | new -> next
  // current -> new | new -> next
  current->next = new;

  return NODE_SUCCESS;
}

int delete_node(Node **head, const char *id) {
  // no input
  if (head == NULL || id == NULL) {
    return NODE_ERROR_BAD_PARAM;
  }

  // there's no node in the list, can't perform deletion
  if (*head == NULL) {
    return NODE_ERROR_BAD_PARAM;
  }

  Node *current = *head;
  Node *prev = NULL;

  while (current != NULL) {
    if (strcmp(current->data.id, id) == 0) {
      if (prev == NULL) {
        // move head of list to next one
        *head = current->next;
      } else {
        // move previous' next to current's next
        prev->next = current->next;
      }

      free_event(&current->data);
      free(current);

      return NODE_SUCCESS;
    }
    prev = current;
    current = current->next;
  }

  return NODE_ERROR_NOT_FOUND;
}

void range_and_print_node(Node **head, char *date1, char *date2) {
  if (!(head && *head && date1 && date2)) {
    printf("Bad parameters!");
    return;
  }

  // make sure the date section is early to latest
  if (strcmp(date1, date2) > 0) {
    char *temp = date1;
    date1 = date2;
    date2 = temp;
  }

  Node *current = *head;
  int count = 0;

  while (current != NULL) {
    if (strcmp(current->data.date, date1) >= 0 &&
        strcmp(current->data.date, date2) <= 0) {
      print_event(&current->data);
      count++;
    }
    current = current->next;
  }

  printf("found %d event%s\n", count, count > 1 ? "s" : "");
}

Node* find_node_by_id(Node **head, const char *id) {
  if (!(head && *head && id)) {
    return NULL;
  }

  Node *current = *head;
  while (current != NULL) {
    if (strcasecmp(current->data.id, id) == 0) {
      return current;
    }
    current = current->next;
  }

  return NULL;
}

void find_and_print_node(Node **head, const char *keyword) {
  if (!(head && *head && keyword)) {
    return;
  }

  int count = 0;
  Node *current = *head;
  while (current != NULL) {
    if (strcasestr(current->data.vehicle, keyword) ||
        strcasestr(current->data.mission, keyword)) {
      count++;
      print_event(&current->data);
    }
    current = current->next;
  }

  printf("%d matched case%s\n", count, count > 2 ? "s" : "");
}

void free_list(Node **head) {
  if (head == NULL || *head == NULL) {
    return;
  }

  Node *current = *head;

  while (current != NULL) {
    Node *next = current->next;

    free_event(&current->data);

    free(current);

    current = next;
  }

  *head = NULL;
}

void print_list(const Node *head) {
  if (head == NULL) {
    printf("No item in the list!!\n");
    return;
  }

  int count = 0;
  const Node *current = head;
  while (current != NULL) {
    count++;

    Event event = current->data;
    print_event(&event);
    current = current->next;
  }

  printf("Items in the list: %d\n", count);
}
