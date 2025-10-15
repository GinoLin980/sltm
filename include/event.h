#ifndef EVENT_H
#define EVENT_H

#define MAX_ID_LEN 11
#define MAX_DATE_LEN 11 // "xxxx-xx-xx\0"
#define MAX_VEHICLE_LEN 100
#define MAX_MISSION_LEN 100
#define MAX_SITE_LEN 100

typedef enum {
  SCHEDULED, //
  SUCCESS,   //
  FAILURE,   //
  DELAYED,   //
  CANCELLED  //
} Status;

typedef struct {
  char id[MAX_ID_LEN];
  char date[MAX_DATE_LEN];
  char vehicle[MAX_VEHICLE_LEN];
  char mission[MAX_MISSION_LEN];
  char site[MAX_SITE_LEN];
  Status status;
} Event;

// parse the row csv string into Event
Event *csv2event(char *csv_row, int *status);
Event *new_event(const char id[MAX_ID_LEN], const char date[MAX_DATE_LEN],
                 const char vehicle[MAX_VEHICLE_LEN],
                 const char mission[MAX_MISSION_LEN], Status status);
Status str2status(const char *status);

#endif // !EVENT_H
