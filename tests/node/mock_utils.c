#include <stdbool.h>
#include <string.h>

// split by "-" and use atoi to compare numbers
// true if date1 > date2, false if smaller
bool date_cmp(const char *date1, const char *date2) {
  if (date1 == NULL || date2 == NULL) {
    return false;
  }

  // Simple string comparison works for "YYYY-MM-DD" format
  // because lexicographic order matches chronological order
  return strcmp(date1, date2) > 0;
}
