#include <stdlib.h>

#ifndef TABLE_H
#define TABLE_H

typedef struct Myelement {
	double num;
}ele;

typedef struct Myele_list {
	struct Myele_list* next;
	ele* element;
}ele_list;

typedef struct MyHead_ele_List {
	ele_list* head;
	char* name;
}head_ele_list;

typedef struct MyTable {
	int	col_size;
	int	row_size;
	head_ele_list* col_head;
	head_ele_list* row_head;
}Table;

Table* Generate_Table();

ele* table(Table* p, int x, int y);

bool table_row_swap(Table* p, int l, int r);

bool table_col_swap(Table* p, int l, int r);

bool table_del_row(Table* p, int l);

bool table_del_col(Table* p, int l);

bool table_add_row(Table* p, int l, char* name);

bool table_add_col(Table* p, int l, char* name);

char* query_row_name(Table* p, int l);

char* query_col_name(Table* p, int l);

#endif 