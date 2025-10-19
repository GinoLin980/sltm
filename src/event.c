#include "../include/event.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

Event *new_event(const char *id, const char *date, const char *vehicle,
                   const char *mission, const char *site, Status status) {

    if (date == NULL || strlen(date) != 10) {
        fprintf(stderr, "Invalid data: Date is NULL or length is not 10.\n");
        return NULL;
    }

    if (!validate_date(date)) {
        fprintf(stderr, "Invalid date: %s\n", date);
        return NULL;
    }

    if (id == NULL || strlen(id) == 0) {
        fprintf(stderr, "Invalid data: ID is empty.\n");
        return NULL;
    }

    if (vehicle == NULL || strlen(vehicle) == 0) {
        fprintf(stderr, "Invalid data: Vehicle is empty.\n");
        return NULL;
    }

    if (site == NULL || strlen(site) == 0) {
        fprintf(stderr, "Invalid data: Site is empty.\n");
        return NULL;
    }

    if (mission == NULL || strlen(mission) == 0) {
        fprintf(stderr, "Invalid data: Mission is empty.\n");
        return NULL;
    }

    if (status == UNKNOWN) {
        fprintf(stderr, "Invalid data: Status is UNKNOWN.\n");
        return NULL;
    }

    Event *event = malloc(sizeof(Event));
    if (!event) {
        perror("Failed to allocate memory for Event");
        return NULL;
    }

    event->id = strdup(id);
    event->date = strdup(date);
    event->vehicle = strdup(vehicle);
    event->mission = strdup(mission);
    event->site = strdup(site);
    event->status = status;
    
    if (!event->id || !event->date || !event->vehicle || !event->site || !event->mission) {
        fprintf(stderr, "Failed to allocate memory for event strings\n");
        free_event(event); 
        return NULL;
    }
    
    return event;
}

Event *csv2event(char *csv_row, int *status) {
    if (csv_row == NULL || status == NULL) {
        *status = -1;
        return NULL;
    }

    // Initialize status to success
    *status = 0;

    // Make a copy of the csv_row since strtok modifies the string
    char *row_copy = strdup(csv_row);
    if (row_copy == NULL) {
        *status = -1;
        return NULL;
    }

    // Parse CSV fields: id,date,vehicle,mission,site,status
    char *fields[6];
    int field_count = 0;
    char *token = strtok(row_copy, ",");
    
    while (token != NULL && field_count < 6) {
        // Trim leading/trailing whitespace
        while (isspace((unsigned char)*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }
        
        fields[field_count++] = token;
        token = strtok(NULL, ",");
    }

    // Check if we have all 6 fields
    if (field_count != 6) {
        fprintf(stderr, "Invalid CSV format: expected 6 fields, got %d\n", field_count);
        free(row_copy);
        *status = -1;
        return NULL;
    }

    // Parse status string
    Status event_status = str2status(fields[5]);

    // Create the event using new_event
    Event *event = new_event(fields[0], fields[1], fields[2], fields[3], fields[4], event_status);
    
    free(row_copy);

    if (event == NULL) {
        *status = -1;
        return NULL;
    }

    return event;
}

Status str2status(const char *status_str) {
    if (status_str == NULL) {
        return UNKNOWN;
    }

    if (strcasecmp(status_str, "Scheduled") == 0) {
        return SCHEDULED;
    } else if (strcasecmp(status_str, "Success") == 0) {
        return SUCCESS;
    } else if (strcasecmp(status_str, "Failure") == 0) {
        return FAILURE;
    } else if (strcasecmp(status_str, "Delayed") == 0) {
        return DELAYED;
    } else if (strcasecmp(status_str, "Cancelled") == 0) {
        return CANCELLED;
    } else {
        return UNKNOWN;
    }
}
void print_event(Event *event) {
    if (event == NULL) return; 

    printf("ID: %s\n", event->id);
    printf("Date: %s\n", event->date);
    printf("Vehicle: %s\n", event->vehicle);
    printf("Mission: %s\n", event->mission);
    printf("Site: %s\n", event->site);

    char *status_str;
    switch (event->status) {
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
}
void free_event(Event *event) {
    if (event == NULL) return;
    free(event->id);
    free(event->date);
    free(event->vehicle);
    free(event->mission);
    free(event->site);
}
