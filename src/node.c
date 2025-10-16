#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/node.h"
#include "../include/utils.h"

Node *new_node(const Event *event) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {return NULL;}

    n->data = *event;
    n->next = NULL;

    return n;
}

int insert_node(Node **head, const Event *event) {
    // validate input, addresses aren't passed
    if (head == NULL || event == NULL) {
        return -1;
    }

    Node *new = new_node(event);

    // the list is empty(Node* is NULL) or `new.date` is smaller than first item in list
    if (*head == NULL || date_cmp((*head)->data.date, new->data.date)) {
        new->next = *head; // point `new.next` to original `head` or `NULL`
        *head = new; // let `new` being NEW `head`
        return 0;
    }

    // copy of `*head`
    Node *current = *head;

    while (current->next != NULL) {
        // new's date is smaller than current's next's date
        // therefore we want to insert between current and current's next
        if (date_cmp(current->next->data.date, new->data.date)) {
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

    return 0;
}

int delete_node(Node **head, const char *id) {
    // no input
    if (head == NULL || id == NULL) {
        return -1;
    }

    // there's no node in the list, can't perform deletion
    if (*head == NULL) {
        return -1;
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
            
            return 0;
        }
        prev = current;
        current = current->next;
    }

    return -1;
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
        printf("No item in the list!!");
        return;
    }

    int count = 0;
    Node *current = head;
    while (current != NULL) {
        count++;

        Event event = current->data;
        printf("ID: %s\n", event.id);
        printf("Date: %s\n", event.date);
        printf("Vehicle: %s\n", event.vehicle);
        printf("Mission: %s\n", event.mission);
        printf("Site: %s\n", event.site);

        char *status_str;
        switch (event.status)
        {
        case SUCCESS:
            status_str = "Success";
            break;
        case SCHEDULED:
            status_str = "Scheduled";
            break;
        case FAILURE:
            status_str = "Failure";
            break;
        case DELAYED:
            status_str = "Delayed";
            break;
        case CANCELLED:
            status_str = "Cancelled";
            break;
        default:
            status_str = "Unknown";
            break;
        }
        printf("Status: %s\n", status_str);
        printf("--------------------\n");

        current = current->next;
    }

    printf("Items in the list: %d", count);
}
