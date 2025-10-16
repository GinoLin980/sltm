#include "commands.h"
#include "csv.h"
#include "event.h"   
#include "node.h"    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper: convert string to lowercase

static void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}


CommandResult cmd_load(Node **head, char **args, int argc) {
    if (argc < 1) {
        printf("Error: Missing filename\n");
        printf("Usage: load <filename>\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *filename = args[0];
    printf("Loading from file: %s (simulation for now)\n", filename);

    Event *e = new_event("TEST001", "2025-10-16", "Falcon 9", "Starlink 99", "CCSFS", SUCCESS);
    if (e == NULL) {
        printf("Failed to allocate Event\n");
        return CMD_ERROR_PARSING_FAILED;
    }

    // insert_node to attach to linked list
    int result = insert_node(head, e);
    if (result != 0) {
        printf("Failed to insert node\n");
        free_event(e);
        free(e);
        return CMD_ERROR_NOT_FOUND;
    }

    printf("Loaded event %s successfully into list\n", e->id);
    return CMD_SUCCESS;
}


CommandResult cmd_add(Node **head, char **args, int argc) {
    if (argc < 6) {
        printf("Error: Missing arguments\n");
        printf("Usage: add <id> <date> <vehicle> <mission> <site> <status>\n");
        return CMD_ERROR_INVALID_ARGS;
    }

   Convert status text to enum
    Status status;
    if (strcasecmp(args[5], "SUCCESS") == 0)
        status = SUCCESS;
    else if (strcasecmp(args[5], "DELAYED") == 0)
        status = DELAYED;
    else
        status = SCHEDULED;

use the real new_event() function
    Event *e = new_event(args[0], args[1], args[2], args[3], args[4], status);
    if (e == NULL) {
        printf("Failed to create event\n");
        return CMD_ERROR_PARSING_FAILED;
    }

    int result = insert_node(head, e);
    if (result != 0) {
        printf("Failed to insert event %s\n", args[0]);
        free_event(e);
        free(e);
        return CMD_ERROR_NOT_FOUND;
    }

    printf("Added event %s successfully\n", args[0]);
    return CMD_SUCCESS;
}

// cmd_help
CommandResult cmd_help(Node **head, char **args, int argc) {
    (void)head; (void)args; (void)argc;

    printf("\nAvailable commands:\n");
    printf("  load <file>                     - Load events (simulated)\n");
    printf("  add <id> <date> <vehicle> <mission> <site> <status>\n");
    printf("                                  - Add new event to timeline\n");
    printf("  help                            - Show this help message\n\n");
    return CMD_SUCCESS;
}


CommandResult execute_command(Node **head, const char *input) {
    if (input == NULL || strlen(input) == 0)
        return CMD_SUCCESS;

    // remove newline
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
        ((char *)input)[len - 1] = '\0';

    char buffer[256];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    to_lowercase(buffer);

    char *args[10];
    int argc = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL && argc < 10) {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (argc == 0) return CMD_SUCCESS;
    const char *cmd_name = args[0];

    int cmd_count;
    const Command *commands = get_all_commands(&cmd_count);

    for (int i = 0; i < cmd_count; i++) {
        if (strcmp(cmd_name, commands[i].name) == 0) {
            return commands[i].handler(head, args + 1, argc - 1);
        }
    }

    printf("Unknown command: %s\n", cmd_name);
    return CMD_ERROR_UNKNOWN_COMMAND;
}


const Command *get_all_commands(int *count) {
    static const Command commands[] = {
        {"load", cmd_load, "load <filename>", 1},
        {"add",  cmd_add,  "add <id> <date> <vehicle> <mission> <site> <status>", 6},
        {"help", cmd_help, "help", 0}
    };
    *count = sizeof(commands) / sizeof(commands[0]);
    return commands;
}


int main(void) {
    Node *head = NULL;
    char input[100];

    printf("Simple test (load, add, help)\n");
    printf("Type 'help' to see commands, 'exit' to quit.\n\n");

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin))
            break;
        char temp[100];
        strncpy(temp, input, sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        to_lowercase(temp);
        if (strncmp(temp, "exit", 4) == 0)
            break;
        execute_command(&head, input);
    }

    free_list(&head);
    printf("Goodbye!\n");
    return 0;
}
