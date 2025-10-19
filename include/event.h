#ifndef EVENT_H
#define EVENT_H

typedef enum {
  SCHEDULED, //
  SUCCESS,   //
  FAILURE,   //
  DELAYED,   //
  CANCELLED, //
  UNKNOWN    //
} Status;

typedef struct {
  char *id;
  char *date;
  char *vehicle;
  char *mission;
  char *site;
  Status status;
} Event;

// parse the row csv string into Event
Event *csv2event(char *csv_row, int *status);
Event *new_event(const char *id, const char *date, const char *vehicle,
                 const char *mission, const char *site, Status status);
Status str2status(const char *status_str);
void print_event(Event *event);
void free_event(Event *event);

#endif // !EVENT_H
