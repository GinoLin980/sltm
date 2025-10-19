// NOTE: AI generated, human reviewed
#include "../../include/event.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> // for strcasecmp

// Create a new Event with dynamically allocated strings
Event *new_event(const char *id, const char *date, const char *vehicle,
                 const char *mission, const char *site, Status status) {
  if (id == NULL || date == NULL || vehicle == NULL || mission == NULL ||
      site == NULL) {
    return NULL;
  }

  Event *e = malloc(sizeof(Event));
  if (e == NULL) {
    return NULL;
  }

  // Allocate and copy strings
  e->id = strdup(id);
  e->date = strdup(date);
  e->vehicle = strdup(vehicle);
  e->mission = strdup(mission);
  e->site = strdup(site);
  e->status = status;

  // Check if any strdup failed
  if (e->id == NULL || e->date == NULL || e->vehicle == NULL ||
      e->mission == NULL || e->site == NULL) {
    free_event(e);
    free(e);
    return NULL;
  }

  return e;
}

// Free all dynamically allocated strings in Event
void free_event(Event *event) {
  if (event == NULL) {
    return;
  }

  free(event->id);
  free(event->date);
  free(event->vehicle);
  free(event->mission);
  free(event->site);

  // Set to NULL to avoid double-free issues
  event->id = NULL;
  event->date = NULL;
  event->vehicle = NULL;
  event->mission = NULL;
  event->site = NULL;
}

void print_event(Event *event) {
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
