#ifndef _Table_h_
#define _Table_h_

#include "graphics.h"
#include "genlib.h"
#include <stdlib.h>

static const int display_bar = 0;
static const int display_line = 1;
static const int display_dot = 2;

typedef struct Myelement {
	double num;
	char num_string[20];
}ele;

typedef struct Myele_list {
	struct Myele_list* next;
	ele* element;
}ele_list;

typedef struct MyHead_ele_List {
	ele_list* head;
	char* name;	
	int display_state;
}head_ele_list;

typedef struct MyTable {
	int	col_size;
	int	row_size;
	head_ele_list* col_head;
	head_ele_list* row_head;
	char* data_name;
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

bool change_row_name(Table* p, int l, char* name);

bool change_col_name(Table* p, int l, char* name);

bool change_data_name(Table* p, char* name);

int query_col_num(Table* p);

int query_row_num(Table* p);

double query_col_average(Table* p, int l);

bool query_if_show_bar(Table* p, int l);

bool query_if_show_line(Table* p, int l);

bool query_if_show_dot(Table* p, int l);

bool let_it_show_bar(Table* p, int l);

bool let_it_show_line(Table* p, int l);

bool let_it_show_dot(Table* p, int l);

bool table_show_bar(Table* p);

bool table_show_line(Table* p);

bool table_show_dot(Table* p);

bool Table_input(Table* p, FILE* fp);

bool Table_output(Table* p, FILE* fp);
#endif 