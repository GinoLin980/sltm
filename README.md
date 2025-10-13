# SLTM

## universal util functions(will reuse in many func)

- `validate date`(for `load <file>`, `add <record>`, `range <start_date> <end_date>`, `update <id>..`) returns `bool`
- `compare date`, assert input is valid, returns bool

## similar concept commands(can be built for one person)

- `add` and `update`

## other functions in consider

- graceful shutdown when receiving signal(ctrl-c), free memory etc.

---

Use the same linked list unless load is called more than one time

---
Make main function stay clean that only main loop in it. And write functions in other headers and sources

---

## Divide functions into different source files

- For `commands.c` `commands.h`, it is responsible for interpret commands and parse arguments, which calls the corresponding function afterward, and the text output.

- `csv.c` `csv.h` are responsible for loading and exporting CSV files

- `event.c` `event.h` are where event and it's operation sits, like `add`(new data, not new node)

- `utils.c` `utils.h` are [universal functions](#universal-util-functionswill-reuse-in-many-func)

- `node.c` `node.h` are for linked list node operation like `add`(connect nodes) `free` etc.
