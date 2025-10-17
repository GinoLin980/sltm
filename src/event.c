#include "../include/event.h"
#include <stdio.h>
#include <string.h>

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
