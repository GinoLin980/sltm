// needed for strdup on some systems
#define _POSIX_C_SOURCE 200809L
#include "csv.h"
#include "event.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 100

// gets rid of spaces and stuff at the beginning/end
// idk if there's already a function for this lol
static char *trim_whitespace(char *str) {
    if (str == NULL) {
        return NULL;
    }

    // move past leading spaces
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') {
        str++;
    }

    if (*str == '\0') {
        return str;
    }

    // now do the end
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    *(end + 1) = '\0';

    return str;
}

// reads the csv and puts each row into an array
char **read_csv(const char *filename, int *count) {
    if (filename == NULL || count == NULL) {
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *count = 0;
        return NULL;
    }

    int capacity = INITIAL_CAPACITY;
    char **rows = malloc(capacity * sizeof(char *));
    if (rows == NULL) {
        fclose(file);
        *count = 0;
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    int row_count = 0;
    int line_number = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;

        // skip the header
        if (line_number == 1) {
            continue;
        }

        char *trimmed = trim_whitespace(line);
        
        // ignore empty lines
        if (strlen(trimmed) == 0) {
            continue;
        }

        // make sure the row is actually valid before adding it
        int status;
        Event *event = csv2event(trimmed, &status);
        // not sure if this is the best way to validate but it works
        
        if (event == NULL || status != 0) {
            if (event != NULL) {
                free_event(event);
                free(event);
            }
            continue;
        }

        // don't need this anymore
        free_event(event);
        free(event);

        // need more space?
        if (row_count >= capacity) {
            capacity *= 2;
            char **new_rows = realloc(rows, capacity * sizeof(char *));
            if (new_rows == NULL) {
                // cleanup if realloc fails
                for (int i = 0; i < row_count; i++) {
                    free(rows[i]);
                }
                free(rows);
                fclose(file);
                *count = 0;
                return NULL;
            }
            rows = new_rows;
        }

        // copy the row into our array
        rows[row_count] = strdup(trimmed);
        if (rows[row_count] == NULL) {
            for (int i = 0; i < row_count; i++) {
                free(rows[i]);
            }
            free(rows);
            fclose(file);
            *count = 0;
            return NULL;
        }
        row_count++;
    }

    fclose(file);
    *count = row_count;
    return rows;
}

// converts status enum to the string version
static const char *status_to_str(Status status) {
    switch (status) {
        case SCHEDULED:
            return "Scheduled";
        case SUCCESS:
            return "Success";
        case FAILURE:
            return "Failure";
        case DELAYED:
            return "Delayed";
        case CANCELLED:
            return "Cancelled";
        default:
            return "Unknown";
    }
}

// writes the linked list back out to a csv file
int export_csv(const char *filename, Node *head) {
    if (filename == NULL) {
        return 1;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 1;
    }

    fprintf(file, "ID,Date,Vehicle,Mission,Site,Status\n");

    // go through each node and write it as a row
    Node *current = head;
    while (current != NULL) {
        Event *event = &current->data;
        
        // putting quotes around everything to be safe
        fprintf(file, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                event->id ? event->id : "",
                event->date ? event->date : "",
                event->vehicle ? event->vehicle : "",
                event->mission ? event->mission : "",
                event->site ? event->site : "",
                status_to_str(event->status));

        current = current->next;
    }

    fclose(file);
    return 0;
}