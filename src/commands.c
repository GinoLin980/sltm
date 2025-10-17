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

CommandResult cmd_update(Node **head, char **args, int argc) {
    if (argc < 2) {
        printf("Error: Missing arguments\n");
        printf("Usage: update <id> <field>=<value> [<field>=<value> ...]\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *id = args[0];
    
    // Step 1: Find existing node
    Node *existing = find_node_by_id(head, id);
    if (existing == NULL) {
        printf("Error: Event with ID '%s' not found\n", id);
        return CMD_ERROR_NOT_FOUND;
    }

    // Step 2: Deep copy the event
    Event *event_copy = new_event(
        existing->data.id,
        existing->data.date,
        existing->data.vehicle,
        existing->data.mission,
        existing->data.site,
        existing->data.status
    );
    
    if (event_copy == NULL) {
        printf("Error: Failed to create event copy\n");
        return CMD_ERROR_NO_MEMORY;
    }

    // Step 3: Parse and apply updates
    int updated = 0;
    for (int i = 1; i < argc; i++) {
        char *arg_copy = strdup(args[i]);
        if (arg_copy == NULL) {
            printf("Error: Memory allocation failed\n");
            free_event(event_copy);
            free(event_copy);
            return CMD_ERROR_NO_MEMORY;
        }
        
        char *equals = strchr(arg_copy, '=');
        if (equals == NULL) {
            printf("Error: Invalid format '%s'. Use field=value\n", args[i]);
            free(arg_copy);
            continue;
        }
        
        *equals = '\0';
        char *field = arg_copy;
        char *value = equals + 1;
        
        // Update corresponding field
        if (strcasecmp(field, "id") == 0) {
            free(event_copy->id);
            event_copy->id = strdup(value);
            if (event_copy->id == NULL) {
                printf("Error: Memory allocation failed\n");
                free(arg_copy);
                free_event(event_copy);
                free(event_copy);
                return CMD_ERROR_NO_MEMORY;
            }
            updated = 1;
        }
        else if (strcasecmp(field, "date") == 0) {
            if (!validate_date(value)) {
                printf("Error: Invalid date format '%s'\n", value);
                free(arg_copy);
                continue;
            }
            free(event_copy->date);
            event_copy->date = strdup(value);
            if (event_copy->date == NULL) {
                printf("Error: Memory allocation failed\n");
                free(arg_copy);
                free_event(event_copy);
                free(event_copy);
                return CMD_ERROR_NO_MEMORY;
            }
            updated = 1;
        }
        else if (strcasecmp(field, "vehicle") == 0) {
            free(event_copy->vehicle);
            event_copy->vehicle = strdup(value);
            if (event_copy->vehicle == NULL) {
                printf("Error: Memory allocation failed\n");
                free(arg_copy);
                free_event(event_copy);
                free(event_copy);
                return CMD_ERROR_NO_MEMORY;
            }
            updated = 1;
        }
        else if (strcasecmp(field, "mission") == 0) {
            free(event_copy->mission);
            event_copy->mission = strdup(value);
            if (event_copy->mission == NULL) {
                printf("Error: Memory allocation failed\n");
                free(arg_copy);
                free_event(event_copy);
                free(event_copy);
                return CMD_ERROR_NO_MEMORY;
            }
            updated = 1;
        }
        else if (strcasecmp(field, "site") == 0) {
            free(event_copy->site);
            event_copy->site = strdup(value);
            if (event_copy->site == NULL) {
                printf("Error: Memory allocation failed\n");
                free(arg_copy);
                free_event(event_copy);
                free(event_copy);
                return CMD_ERROR_NO_MEMORY;
            }
            updated = 1;
        }
        else if (strcasecmp(field, "status") == 0) {
            Status new_status = str2status(value);
            if (new_status == UNKNOWN) {
                printf("Error: Invalid status '%s'\n", value);
                free(arg_copy);
                continue;
            }
            event_copy->status = new_status;
            updated = 1;
        }
        else {
            printf("Error: Unknown field '%s'\n", field);
        }
        
        free(arg_copy);
    }

    if (!updated) {
        printf("No valid fields were updated\n");
        free_event(event_copy);
        free(event_copy);
        return CMD_ERROR_INVALID_ARGS;
    }

    // Step 4: Delete old node
    int del_status = delete_node(head, id);
    if (del_status != NODE_SUCCESS) {
        printf("Error: Failed to delete old node\n");
        free_event(event_copy);
        free(event_copy);
        return CMD_ERROR_PARSING_FAILED;
    }

    // Step 5: Insert updated node
    int ins_status = insert_node(head, event_copy);
    if (ins_status != NODE_SUCCESS) {
        printf("Error: Failed to insert updated node\n");
        // WARNING: Original node is already deleted!
        // This is a critical state - consider re-inserting original
        free_event(event_copy);
        free(event_copy);
        return CMD_ERROR_PARSING_FAILED;
    }

    printf("Event '%s' updated successfully\n", event_copy->id);
    
    // Step 6: Free the temporary copy
    free_event(event_copy);
    free(event_copy);

    return CMD_SUCCESS;
}
    
// cmd_delete
CommandResult cmd_delete(Node **head, char **args, int argc) {
    if (argc < 1) {
        printf("Error: Missing ID\n");
        printf("Usage: delete <id>\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *id = args[0];
    int result = delete_node(head, id);
    
    if (result == 0) {
        printf("Deleted event %s successfully\n", id);
        return CMD_SUCCESS;
    } else {
        printf("Error: Event with ID %s not found\n", id);
        return CMD_ERROR_NOT_FOUND;
    }
}

// cmd_range
CommandResult cmd_range(Node **head, char **args, int argc) {
    if (argc < 2) {
        printf("Error: Missing date range\n");
        printf("Usage: range <start_date> <end_date>\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *start_date = args[0];
    const char *end_date = args[1];
    
    if (!valid_date(start_date) || !valid_date(end_date)) {
        printf("Error: Invalid date format. Use YYYY-MM-DD\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    if (strcmp(start_date, end_date) > 0) {
        printf("Error: Start date cannot be after end date\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    range_and_print_node(head, (char*)start_date, (char*)end_date);
    return CMD_SUCCESS;
}

// cmd_find
CommandResult cmd_find(Node **head, char **args, int argc) {
    if (argc < 1) {
        printf("Error: Missing keyword\n");
        printf("Usage: find <keyword>\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *keyword = args[0];

    find_and_print_node(head, keyword);
    return CMD_SUCCESS;

}

// cmd_export
CommandResult cmd_export(Node **head, char **args, int argc) {
    if (argc < 1) {
        printf("Error: Missing filename\n");
        printf("Usage: export <filename>\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *filename = args[0];
    int result = export_csv(filename, *head);
    
    if (result == 0) {
        printf("Successfully exported events to %s\n", filename);
        return CMD_SUCCESS;
    } else {
        printf("Error: Failed to export to file %s\n", filename);
        return CMD_ERROR_FILE_NOT_FOUND;
    }
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
