#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// validate if date conform "xxxx-xx-xx"
bool valid_date(const char *date);
// split by "-" and use atoi to compare numbers
// true if date1 > date2, false if smaller
bool date_cmp(const char *date1, const char *date2);

#endif // !UTILS_H
