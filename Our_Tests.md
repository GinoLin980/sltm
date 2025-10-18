# Our Tests

Our group made 6 tests using the space_launches.csv file we created.

Dataset has 34 total entries: 27 valid launch records and 7 invalid ones for testing error handling

The invalid entries have different problems:
- X001: has "xx" in the date field
- X002: vehicle field is empty
- X003: status field is empty
- X004: date is in wrong format (MM-DD-YYYY)
- X005: month 13 (doesn't exist)
- X006: Feb 30 (impossible date)
- X007: day 32 (out of range)

## Test 1 - Load the CSV

Run these commands:
```
./sltm
load space_launches.csv
```

Should print each event as it loads, then show summary:
```
Loaded event SLV001 successfully into list
Loaded event SLV002 successfully into list
...
Loaded 34 events
Valid: 27
Invalid: 7
```

This tests if the program correctly filters out bad data when loading.

## Test 2 - Range Query
```
./sltm
load space_launches.csv
range 2025-03-01 2025-04-30
```

Should show 6 launches from March and April with all their details (ID, Date, Vehicle, Mission, Site, Status).

Expected events in range:
- SLV005 (March 1) - Falcon Heavy
- SLV006 (March 10) - Electron
- SLV007 (March 25) - Starship
- SLV008 (April 2) - Vega C
- SLV009 (April 12) - PSLV
- SLV010 (April 30) - Falcon 9

Tests the date range filtering.

## Test 3 - Find Command
```
./sltm
load space_launches.csv
find starlink
```

Should find and display 6 Starlink missions with full details for each:
- SLV001 - Starlink Group 9-1
- SLV010 - Starlink Group 9-2
- SLV013 - Starlink Group 9-3
- SLV019 - Starlink Group 10-1
- SLV022 - Starlink Group 10-2
- SLV026 - Starlink Group 10-3

Should print "6 matched cases" at the end.

This checks if the search works and is case-insensitive.

## Test 4 - Adding a Record
```
./sltm
load space_launches.csv
add "TEST01,2025-03-20,Falcon 9,Test Mission,CCSFS SLC-40,Success"
range 2025-03-10 2025-03-25
```

Should print:
```
Added event TEST01 successfully
```

Then the range query should show TEST01 in the right spot:
- SLV006 (March 10)
- TEST01 (March 20) <- should be here
- SLV007 (March 25)

Makes sure new records get inserted in order by date.

## Test 5 - Updating a Record
```
./sltm
load space_launches.csv
update SLV005 status=Success
find SLV005
```

Should print:
```
Event 'SLV005' updated successfully
```

Then when we find SLV005, its status should show "Success" instead of "Scheduled". Everything else stays the same.

Tests if we can modify records without breaking them.

## Test 6 - Deleting a Record
```
./sltm
load space_launches.csv
delete SLV010
find starlink
```

Should print:
```
Deleted event SLV010 successfully
```

After deleting SLV010, searching for starlink should only find 5 missions (not 6 anymore). SLV010 shouldn't show up.

Checks if delete actually removes records from the list.

## How to Run These Tests

Compile first:
```
make
```

Then run the program:
```
./sltm
```

Type the commands from each test. Use Ctrl-C to quit.

To check for memory leaks use valgrind:
```
valgrind --leak-check=full ./sltm
```

Run some commands then exit with Ctrl-C.

## Notes

- All tests need the space_launches.csv file loaded first
- The list stays sorted by date automatically
- Run tests in order for best results
- When using add command, remember to use quotes around the CSV record
