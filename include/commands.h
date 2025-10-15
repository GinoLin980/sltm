
#ifndef COMMANDS_H
#define COMMANDS_H

#include "node.h"

// easy name to remember
typedef enum {
  CMD_SUCCESS = 0,
  CMD_ERROR_INVALID_ARGS,
  CMD_ERROR_FILE_NOT_FOUND,
  CMD_ERROR_PARSING_FAILED,
  CMD_ERROR_NOT_FOUND,
  CMD_ERROR_UNKNOWN_COMMAND
} CommandResult;

// define a function pointer type called CommandHandler that return
// CommandResult and takes 3 parameters
typedef CommandResult (*CommandHandler)(Node **head, char **args, int argc);

typedef struct {
  const char *name;       // name of commands(e.g. "load", "add")
  CommandHandler handler; // pointer of function
  const char *usage;      // help texts
  int min_args;           // least count of args for early exit
} Command;

// interface of the interactive commands, use same parameters for easier call
// and fit the CommandHandler
CommandResult cmd_load(Node **head, char **args, int argc);
CommandResult cmd_add(Node **head, char **args, int argc);
CommandResult cmd_update(Node **head, char **args, int argc);
CommandResult cmd_delete(Node **head, char **args, int argc);
CommandResult cmd_range(Node **head, char **args, int argc);
CommandResult cmd_find(Node **head, char **args, int argc);
CommandResult cmd_export(Node **head, char **args, int argc);
CommandResult cmd_help(Node **head, char **args, int argc);

// main process function
// parse user input and execute target command
CommandResult execute_command(Node **head, const char *input);

// get the array of `Command` and the count of the array
const Command *get_all_commands(int *count);

#endif
