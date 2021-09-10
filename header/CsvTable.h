#ifndef CSV_TABLE_H
#define CSV_TABLE_H

#include <stdio.h>
#include "Object.h"

#define csv_table_add_line(table, pattern, ...) {char buf[500] = {'\0'}; 		\
											sprintf(buf, pattern, __VA_ARGS__); \
											__csv_table_add_line(table, buf)}


typedef struct _scv_table ScvTable;

MEMORY_ALLOCATOR(ScvTable);
CONSTRUCTOR(ScvTable, char* table_name, unsigned tables_amount, ...);

csv_table_change_name();
__csv_table_add_line(ScvTable* table, char* format_string);
__csv_table_set_cell()

#endif
