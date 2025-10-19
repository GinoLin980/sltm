#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "../include/commands.h"
#include "../include/node.h"

// Global pointer to the linked list head for signal handler access
static Node **g_head = NULL;

// Signal handler for Ctrl+C (SIGINT)
void signal_handler(int signum) {
    (void)signum; // Suppress unused parameter warning
    
    printf("\n\nReceived interrupt signal (Ctrl+C)\n");
    printf("Cleaning up and exiting...\n");
    
    // Free the linked list if it exists
    if (g_head != NULL && *g_head != NULL) {
        free_list(g_head);
        printf("Memory freed successfully.\n");
    }
    
    printf("Goodbye!\n");
    exit(0);
}

int main(void) {
    Node *head = NULL;
    g_head = &head;  // Set global pointer for signal handler
    
    // Register signal handler for Ctrl+C
    signal(SIGINT, signal_handler);
    
    printf("===========================================\n");
    printf("  Space Launch Tracking Manager (SLTM)\n");
    printf("===========================================\n");
    printf("Type 'help' to see available commands\n");
    printf("Type 'exit' to quit or press Ctrl+C\n\n");
    
    char input[512];
    
    while (1) {
        printf("\nsltm> ");
        fflush(stdout);  // Ensure prompt is displayed
        
        if (!fgets(input, sizeof(input), stdin)) {
            // EOF or error
            break;
        }
        
        // Remove trailing newline
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        // Check for exit command
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }
        
        // Skip empty input
        if (strlen(input) == 0) {
            continue;
        }
        
        // Execute the command
        execute_command(&head, input);
    }
    
    // Clean up before exit
    printf("\nCleaning up...\n");
    free_list(&head);
    printf("Goodbye!\n");
    
    return 0;
}
