#include <string.h>
#include "event.h"

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