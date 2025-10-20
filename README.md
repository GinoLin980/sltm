# SLTM - Space Launch Tracking Manager

A simple C program for managing space launch records using linked lists.

## Features

- **Add, update, and manage space launch records**
- **Load and export data from CSV files**
- **Search and filter launches by date ranges**
- **Interactive command-line interface**
- **Memory management with linked lists**

## Project Structure

```
├── include
│   ├── commands.h
│   ├── csv.h
│   ├── event.h
│   ├── node.h
│   └── utils.h
├── src
│   ├── commands.c
│   ├── csv.c
│   ├── event.c
│   ├── main.c
│   ├── node.c
│   └── utils.c
├── Makefile
├── memory_results.txt
├── Our_Tests.md
├── README.md
├── space_launches.csv
├── test_1.sh
├── test_2.sh
├── test_3.sh
├── test_4.sh
├── test_5.sh
└── test_6.sh
```
 
## Building and Running

### Build the project:
```bash
make
```

### Run the program:
```bash
./sltm
```

### Available Commands:
- `help` - Show available commands
- `load <filename>` - Load data from CSV file
- `add <record>` - Add a new launch record
- `update <id> <field>=<value>` - Update a launch record
- `delete <id>` - delete a launch record by search id
- `find <vehicle/mission>` - insensitive search on vehicle or mission
- `range <start_date> <end_date>` - Show launches in date range
- `export <filename>` - Save data to CSV file
- `exit` - Quit the program(or use `ctrl-c`)

## Example Usage

```
sltm> load space_launches.csv
sltm> add "L2024-001,2024-03-15,Falcon 9,Starlink-6L,CCSFS,SUCCESS"
sltm> range 2024-01-01 2024-12-31
sltm> exit
```

## Data Structure

Each space launch record contains:
- **ID**: Unique identifier
- **Date**: Launch date (YYYY-MM-DD)
- **Vehicle**: Rocket/spacecraft name
- **Mission**: Mission name
- **Site**: Launch site
- **Status**: Launch status (SCHEDULED, SUCCESS, FAILURE, DELAYED, CANCELLED)

## Notes

- Uses linked lists for dynamic data storage
- Implements proper memory management
- Includes graceful handling of Ctrl+C interruption
- Follows modular C programming practices
