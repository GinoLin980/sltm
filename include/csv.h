#ifndef CSV_H
#define CSV_H

#include "node.h"

// read file and filter valid cases, return array of string
// will modify input ptr of `count` to tell how many rows read
// need to use `free` in signal
char **read_csv(const char *filename, int *count);
// create or override the file, return status, 0 for ok, 1 for error
int export_csv(const char *filename, Node *head);

#endif // !CSV_H
