#include <string.h>
#include "event.h"

Status str2status(const char *status_str)
{
    if (status_str == NULL)
    {
        return UNKNOWN;
    }

    if (strcasecmp(status_str, "Scheduled") == 0)
    {
        return SCHEDULED;
    }
    else if (strcasecmp(status_str, "Success") == 0)
    {
        return SUCCESS;
    }
    else if (strcasecmp(status_str, "Failure") == 0)
    {
        return FAILURE;
    }
    else if (strcasecmp(status_str, "Delayed") == 0)
    {
        return DELAYED;
    }
    else if (strcasecmp(status_str, "Cancelled") == 0)
    {
        return CANCELLED;
    }
    else
    {
        return UNKNOWN;
    }
}

Event *new_event(const char *id, const char *date, const char *vehicle,
                 const char *mission, const char *site, Status status)
{
    Event *event = (Event *)malloc(sizeof(Event));
    if (event == NULL)
    {
        perror("Failed to allocate memory for Event struct");
        return NULL;
    }

    event->id = strdup(id);
    event->date = strdup(date);
    event->vehicle = strdup(vehicle);
    event->mission = strdup(mission);
    event->site = strdup(site);
    event->status = status;

    if (!event->id || !event->date || !event->vehicle || !event->mission || !event->site)
    {
        free_event(event);
        return NULL;
    }

    return event;
}

Event *csv2event(char *csv_row, int *result_status)
{
    char *tokens[6];
    char *saveptr;
    int count = 0;

    char *token = strtok_r(csv_row, ",", &saveptr);
    while (token != NULL && count < 6)
    {
        tokens[count++] = token;
        token = strtok_r(NULL, ",", &saveptr);
    }

    if (count != 6)
    {
        *result_status = -1;
        return NULL;
    }

    Status status = str2status(tokens[5]);
    if (status == UNKNOWN)
    {
        *result_status = -1;
        return NULL;
    }

    *result_status = 0;
    return new_event(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], status);
}

void free_event(Event *event)
{
    if (event == NULL)
    {
        return;
    }
    free(event->id);
    free(event->date);
    free(event->vehicle);
    free(event->mission);
    free(event->site);
    free(event);
}