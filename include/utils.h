#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <time.h>

// Validates a date in format "YYYY-MM-DD"
bool validate_date(const char *date_str);

// Compares two date strings in format "YYYY-MM-DD"
// Returns -1 if date1 < date2, 0 if equal, 1 if date1 > date2
int compare_date(const char *date1, const char *date2);

// Sets up signal handler for graceful shutdown
void setup_signal_handler(void);

// Cleans up resources (memory, files, etc.)
void graceful_shutdown(int signal);

#endif // UTILS_H


