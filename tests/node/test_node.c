#include "../../include/event.h"
#include "../../include/node.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test 1: Create and free a single node
void test_new_node() {
  printf("Testing new_node... ");

  Event *e = new_event("TEST001", "2025-01-15", "Falcon 9", "Test Mission",
                       "CCSFS", SUCCESS);

  Node *node = new_node(e);
  assert(node != NULL);
  assert(strcmp(node->data.id, "TEST001") == 0);
  assert(node->next == NULL);

  free_event(&node->data);
  free(node);
  free(e);

  printf("PASSED\n");
}

// Test 2: Insert into empty list
void test_insert_empty_list() {
  printf("Testing insert into empty list... ");

  Node *head = NULL;
  Event *e = new_event("TEST001", "2025-01-15", "Falcon 9", "Test Mission",
                       "CCSFS", SUCCESS);

  int result = insert_node(&head, e);
  assert(result == 0);
  assert(head != NULL);
  assert(strcmp(head->data.id, "TEST001") == 0);

  free_list(&head);
  free(e);
  assert(head == NULL);

  printf("PASSED\n");
}

// Test 3: Insert multiple nodes in sorted order
void test_insert_sorted() {
  printf("Testing sorted insertion... ");

  Node *head = NULL;

  // Insert in random order
  Event *e1 = new_event("TEST001", "2025-03-15", "Falcon 9", "Mission 1",
                        "CCSFS", SUCCESS);
  Event *e2 = new_event("TEST002", "2025-01-10", "Ariane 6", "Mission 2",
                        "Kourou", DELAYED);
  Event *e3 = new_event("TEST003", "2025-05-20", "Starship", "Mission 3",
                        "Boca Chica", SCHEDULED);

  insert_node(&head, e1);
  insert_node(&head, e2);
  insert_node(&head, e3);

  // Verify sorted order (should be: e2, e1, e3)
  assert(strcmp(head->data.id, "TEST002") == 0);             // First
  assert(strcmp(head->next->data.id, "TEST001") == 0);       // Second
  assert(strcmp(head->next->next->data.id, "TEST003") == 0); // Third
  assert(head->next->next->next == NULL);                    // End

  free_list(&head);
  free(e1);
  free(e2);
  free(e3);

  printf("PASSED\n");
}

// Test 4: Delete first node
void test_delete_first_node() {
  printf("Testing delete first node... ");

  Node *head = NULL;
  Event *e1 = new_event("TEST001", "2025-01-15", "Falcon 9", "Mission 1",
                        "CCSFS", SUCCESS);
  Event *e2 = new_event("TEST002", "2025-03-20", "Ariane 6", "Mission 2",
                        "Kourou", DELAYED);

  insert_node(&head, e1);
  insert_node(&head, e2);

  int result = delete_node(&head, "TEST001");
  assert(result == 0);
  assert(strcmp(head->data.id, "TEST002") == 0);
  assert(head->next == NULL);

  free_list(&head);
  free(e1);
  free(e2);

  printf("PASSED\n");
}

// Test 5: Delete middle node
void test_delete_middle_node() {
  printf("Testing delete middle node... ");

  Node *head = NULL;
  Event *e1 = new_event("TEST001", "2025-01-15", "Falcon 9", "Mission 1",
                        "CCSFS", SUCCESS);
  Event *e2 = new_event("TEST002", "2025-03-20", "Ariane 6", "Mission 2",
                        "Kourou", DELAYED);
  Event *e3 = new_event("TEST003", "2025-05-25", "Starship", "Mission 3",
                        "Boca Chica", SCHEDULED);

  insert_node(&head, e1);
  insert_node(&head, e2);
  insert_node(&head, e3);

  int result = delete_node(&head, "TEST002");
  assert(result == 0);
  assert(strcmp(head->data.id, "TEST001") == 0);
  assert(strcmp(head->next->data.id, "TEST003") == 0);
  assert(head->next->next == NULL);

  free_list(&head);
  free(e1);
  free(e2);
  free(e3);

  printf("PASSED\n");
}

// Test 6: Delete non-existent node
void test_delete_not_found() {
  printf("Testing delete non-existent node... ");

  Node *head = NULL;
  Event *e = new_event("TEST001", "2025-01-15", "Falcon 9", "Mission 1",
                       "CCSFS", SUCCESS);

  insert_node(&head, e);

  int result = delete_node(&head, "NOTFOUND");
  assert(result == -1); // Should fail
  assert(head != NULL); // List unchanged

  free_list(&head);
  free(e);

  printf("PASSED\n");
}

// Test 7: Free empty list
void test_free_empty_list() {
  printf("Testing free empty list... ");

  Node *head = NULL;
  free_list(&head); // Should not crash
  assert(head == NULL);

  printf("PASSED\n");
}

// Test 8: Print empty list
void test_print_empty_list() {
  printf("Testing print empty list... \n");

  Node *head = NULL;
  printf("\n");
  print_list(head); // Should print "No item in the list!"

  printf("PASSED\n");
}

void test_print_list() {
  printf("Testing print list... \n");
  Node *head = NULL;
  Event *e1 = new_event("TEST001", "2025-01-15", "Falcon 9", "Mission 1",
                        "CCSFS", SUCCESS);
  Event *e2 = new_event("TEST002", "2025-03-20", "Ariane 6", "Mission 2",
                        "Kourou", DELAYED);
  Event *e3 = new_event("TEST003", "2025-05-25", "Starship", "Mission 3",
                        "Boca Chica", SCHEDULED);

  insert_node(&head, e1);
  insert_node(&head, e2);
  insert_node(&head, e3);

  print_list(head);

  free_list(&head);
  free(e1);
  free(e2);
  free(e3);
}

// Main test runner
int main() {
  printf("=== Running Node Tests ===\n\n");

  test_new_node();
  test_insert_empty_list();
  test_insert_sorted();
  test_delete_first_node();
  test_delete_middle_node();
  test_delete_not_found();
  test_free_empty_list();
  test_print_empty_list();
  test_print_list();

  printf("\n=== All Tests PASSED ===\n");
  return 0;
}
