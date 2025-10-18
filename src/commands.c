#include "../include/commands.h"
#include "../include/csv.h"
#include "../include/event.h"   
#include "../include/node.h"    
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
    int count = 0;
    char **csv_strs = read_csv(filename, &count);
    
    if (count == 0) {
        printf("Can't load file! It might be empty\n");
        if (csv_strs != NULL) {
            free(csv_strs);
        }
        return CMD_ERROR_PARSING_FAILED;
    }

    int invalid_count = 0;
    for (int i = 0; i < count; i++) {
        int status = 0;
        Event *e = csv2event(csv_strs[i], &status);
        
        if (status != 0) {
            invalid_count++;
            if (e != NULL) {
                free_event(e);
                free(e);
            }
            continue;
        }

        int result = insert_node(head, e);
        if (result != 0) {
            printf("Failed to insert node\n");
            free_event(e);
            free(e);
            for (int j = 0; j < count; j++) {
                free(csv_strs[j]);
            }
            free(csv_strs);
            return CMD_ERROR_PARSING_FAILED;
        }

        printf("Loaded event %s successfully into list\n", e->id);
        free_event(e);
        free(e);
    }

    for (int i = 0; i < count; i++) {
        free(csv_strs[i]);
    }
    free(csv_strs);

    printf("Loaded %d events\nValid: %d\nInvalid: %d\n", 
           count, count - invalid_count, invalid_count);

    return CMD_SUCCESS;
}


CommandResult cmd_add(Node **head, char **args, int argc) {
    if (argc < 1) {
        printf("Error: Missing record\n");
        printf("Usage: add \"<id>,<date>,<vehicle>,<mission>,<site>,<status>\"\n");
        return CMD_ERROR_INVALID_ARGS;
    }

    const char *record = args[0];
    int status = 0;
    Event *e = csv2event(record, &status);
    
    if (status != 0 || e == NULL) {
        printf("Error: Invalid record format\n");
        return CMD_ERROR_PARSING_FAILED;
    }

    int result = insert_node(head, e);
    if (result != 0) {
        printf("Failed to insert event %s\n", e->id);
        free_event(e);
        free(e);
        return CMD_ERROR_NOT_FOUND;
    }

    printf("Added event %s successfully\n", e->id);
    free_event(e);
    free(e);
    
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


CommandResult cmd_help(Node **head, char **args, int argc) {
    (void)head; 
    (void)args; 
    (void)argc;

    printf("\nAvailable commands:\n");
    printf("  load <file>                     - Load events from CSV\n");
    printf("  add \"<record>\"                  - Add new event (CSV format)\n");
    printf("  update <id> <field>=<value>     - Update event fields\n");
    printf("  delete <id>                     - Delete event by ID\n");
    printf("  range <start_date> <end_date>   - Show events in date range\n");
    printf("  find <keyword>                  - Search by keyword\n");
    printf("  export <file>                   - Export to CSV file\n");
    printf("  help                            - Show this help message\n\n");
    return CMD_SUCCESS;
}


CommandResult execute_command(Node **head, const char *input) {
    if (input == NULL || strlen(input) == 0)
        return CMD_SUCCESS;

    char buffer[512];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // Remove newline
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    // Parse arguments, handling quoted strings
    char *args[20];
    int argc = 0;
    
    char *p = buffer;
    while (*p && argc < 20) {
        while (*p == ' ') p++;
        if (*p == '\0') break;
        
        if (*p == '"') {
            p++;
            args[argc++] = p;
            while (*p && *p != '"') p++;
            if (*p == '"') {
                *p = '\0';
                p++;
            }
        } else {
            args[argc++] = p;
            while (*p && *p != ' ') p++;
            if (*p) {
                *p = '\0';
                p++;
            }
        }
    }

    if (argc == 0) return CMD_SUCCESS;

    // Convert ONLY command name to lowercase
    char cmd_lower[50];
    strncpy(cmd_lower, args[0], sizeof(cmd_lower) - 1);
    cmd_lower[sizeof(cmd_lower) - 1] = '\0';
    to_lowercase(cmd_lower);

    int cmd_count;
    const Command *commands = get_all_commands(&cmd_count);

    for (int i = 0; i < cmd_count; i++) {
        if (strcmp(cmd_lower, commands[i].name) == 0) {
            return commands[i].handler(head, args + 1, argc - 1);
        }
    }

    printf("Unknown command: %s\n", cmd_lower);
    return CMD_ERROR_UNKNOWN_COMMAND;
}


const Command *get_all_commands(int *count) {
    static const Command commands[] = {
        {"load",   cmd_load,   "load <filename>", 1},
        {"add",    cmd_add,    "add \"<record>\"", 1},
        {"update", cmd_update, "update <id> <field>=<value>", 2},
        {"delete", cmd_delete, "delete <id>", 1},
        {"range",  cmd_range,  "range <start_date> <end_date>", 2},
        {"find",   cmd_find,   "find <keyword>", 1},
        {"export", cmd_export, "export <filename>", 1},
        {"help",   cmd_help,   "help", 0}
    };
    *count = sizeof(commands) / sizeof(commands[0]);
    return commands;
}
