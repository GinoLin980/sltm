#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

// Internal helper: checks if a year is a leap year
static bool is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Validates a date in "YYYY-MM-DD" format
bool validate_date(const char *date_str) {
    if (!date_str || strlen(date_str) != 10 ||
        date_str[4] != '-' || date_str[7] != '-') {
        return false;
    }

    int year, month, day;
    if (sscanf(date_str, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return false;
    }

    if (year < 1900 || month < 1 || month > 12 || day < 1) return false;

    int days_in_month[] = { 31, 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31 };

    // Adjust February for leap year
    if (month == 2 && is_leap_year(year)) {
        if (day > 29) return false;
    } else {
        if (day > days_in_month[month - 1]) return false;
    }

    return true;
}

// Internal helper: parses a date string into a struct tm
static bool parse_date(const char *date_str, struct tm *tm_out) {
    memset(tm_out, 0, sizeof(struct tm));
    int year, month, day;
    if (sscanf(date_str, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return false;
    }
    tm_out->tm_year = year - 1900;
    tm_out->tm_mon = month - 1;
    tm_out->tm_mday = day;
    return true;
}

// Compares two dates in "YYYY-MM-DD" format
int compare_date(const char *date1, const char *date2) {
    struct tm tm1, tm2;

    if (!parse_date(date1, &tm1) || !parse_date(date2, &tm2)) {
        fprintf(stderr, "Invalid date format in compare_date\n");
        return 0;
    }

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

// Function called when a signal like SIGINT (Ctrl+C) is received
void graceful_shutdown(int signal) {
    printf("\n[INFO] Received signal %d. Shutting down gracefully...\n", signal);

    // Clean up memory, close files, etc.
    // fclose(log_file);
    // free(buffer);

    exit(0);
}

// Sets up signal handlers for graceful shutdown
void setup_signal_handler(void) {
    struct sigaction sa;
    sa.sa_handler = graceful_shutdown;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Capture common termination signals
    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGTERM, &sa, NULL);  // Termination request
}




