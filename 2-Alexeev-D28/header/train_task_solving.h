#ifndef TRAIN_TASK_SOLVING_H
#define TRAIN_TASK_SOLVING_H
#include <stdio.h>
#include "graph.h"

Graph* parse_train_spec_to_graph(FILE* input_stream);
int organize_train_carriges(const char* train_spec_file, const char* soling_file);

#endif
