#include "genlib.h" /*import bool*/
#include "Table.h"
#include <string.h>

Table* Generate_Table() { // O(1)
	Table* p;
	if ((p = (Table*)malloc(sizeof(Table))) == NULL)
		return NULL;
	p->row_size = 0;
	p->col_size = 0;

	if ((p->row_head= (head_ele_list*)malloc(sizeof(head_ele_list))) == NULL)
		return NULL;
	if ((p->col_head = (head_ele_list*)malloc(sizeof(head_ele_list))) == NULL)
		return NULL;
	//if ((p->Pool = (ele*)malloc(sizeof(ele))) == NULL)
	//	return NULL;
	//(p->Pool)->next = 0;
	return p;
};

static ele* query_ele(ele_list* p, int n) { // O(n)
	for (; n; n--, p = p->next);
	return p->element;
}

ele* table(Table* p, int x, int y) { // O(n)
	if (p->row_size < x || x < 1)
		return NULL;
	if (p->col_size < y || y < 1)
		return NULL;
	return query_ele((p->row_head+x)->head, y);
}

bool table_row_swap(Table* p, int l, int r) { // O(n)
	if (l < 1 || l > p->row_size)
		return FALSE;
	if (r < 1 || r > p->row_size)
		return FALSE;
	ele_list* temp;
	temp = (p->row_head+l)->head;
	(p->row_head+l)->head = (p->row_head+r)->head;
	(p->row_head+r)->head = temp;
	return TRUE;
}

bool table_col_swap(Table* p, int l, int r) { // O(n)
	if (l < 1 || l > p ->col_size)
		return FALSE;
	if (r < 1 || r > p->col_size)
		return FALSE;
	ele_list* temp;
	temp = (p->col_head + l)->head;
	(p->col_head + l)->head = (p->col_head + r)->head;
	(p->col_head + r)->head = temp;
	return TRUE;
}

static bool delete_nth_ele(ele_list* p, int n) { //O(n)
	if (n <= 0)
		return FALSE;
	n--;
	for (; p != NULL && n; p = p->next, n--);

	if (p == NULL || p -> next == NULL)
		return FALSE;
	p->next = (p->next)->next;
	return TRUE;
}

static bool add_nth_ele(ele_list* p, ele* element, int n) {// O(n)
	if (n <= 0)
		return FALSE;
	n--;
	for (; p != NULL && n; p = p->next, n--);
	
	if (p == NULL)
		return FALSE;
	ele_list* q;
	if ((q = (ele_list*)malloc(sizeof(ele_list))) == NULL)
		return FALSE;
	q->element = element;
	q->next = p->next;
	p->next = q;
	return TRUE;
}

bool table_del_row(Table* p, int l) { //O(n^2)
	if (l < 1 || l > p->row_size)
		return FALSE;
	head_ele_list* temp;
	if ((temp = (head_ele_list*)malloc(sizeof(head_ele_list) * (p->row_size))) == NULL)
		return FALSE;
	int i;
	for (i = 0; i < l; ++i) {
		temp[i] = p->row_head[i];
	}
	for (i = l; i < p->row_size; ++i) {
		temp[i] = p->row_head[i+1];
	}
	p->row_head = temp;
	p->row_size--;

	for (i = 1; i <= p->col_size; ++i) {
		if (delete_nth_ele((p->col_head + i)->head, l) == FALSE)
			return FALSE;
	}
	return TRUE;
}

bool table_del_col(Table* p, int l) { //O(n^2)
	if (l < 1 || l > p->col_size)
		return FALSE;
	head_ele_list* temp;
	if ((temp = (head_ele_list*)malloc(sizeof(head_ele_list) * (p->col_size))) == NULL)
		return FALSE;
	int i;
	for (i = 0; i < l; ++i) {
		temp[i] = p->col_head[i];
	}
	for (i = l; i < p->col_size; ++i) {
		temp[i] = p->col_head[i + 1];
	}
	p->col_head = temp;
	p->col_size--;

	for (i = 1; i <= p->row_size; ++i) {
		if (delete_nth_ele((p->row_head + i)->head, l) == FALSE)
			return FALSE;
	}
	return TRUE;
}

static bool create_line(head_ele_list *p, int l, char* name) { //O(n)
	if ((p = (head_ele_list*)malloc(sizeof (head_ele_list))) == NULL)
		return FALSE;
	if ((p->name = (char*)malloc(sizeof(char) * strlen(name))) == NULL)
		return FALSE;
	strcpy(p->name, name, sizeof name);
	if ((p -> head = (ele_list *)malloc(sizeof (ele_list))) == NULL)
		return FALSE;
	int i;
	ele_list* pp = p->head;
	for (i = 0; i < l; i++) {
		ele_list* qq;
		if ((qq= (ele_list*)malloc(sizeof(ele_list))) == NULL)
			return FALSE;
		qq->next = NULL;
		if ((qq->element = (ele*)malloc(sizeof(ele))) == NULL)
			return FALSE;
		(qq->element)->num = 0; // default = 0
		pp->next = qq;
		qq->next = NULL;
		pp = qq;
	}
	return TRUE;
}

bool table_add_row(Table* p, int l, char *name) { //O(n^2)
	if (l < 1 || l > p->row_size + 1)
		return FALSE;
	head_ele_list* temp;
	if ((temp = (head_ele_list*)malloc(sizeof(head_ele_list) * (p->row_size + 2))) == NULL)
		return FALSE;
	int i;
	for (i = 0; i < l; ++i) {
		temp[i] = p->row_head[i];
	}
	if (create_line(temp+l, p->col_size, name) == FALSE)
		return FALSE;
	for (i = l; i <= p->row_size; ++i) {
		temp[i + 1] = p->row_head[i];
	}
	p->row_head = temp;
	p->row_size++;

	for (i = 1; i <= p->col_size; ++i) {
		add_nth_ele((p->col_head + i)->head, query_ele((p->row_head + l)->head,i), l);
	}
	return TRUE;
}

bool table_add_col(Table* p, int l, char* name) { //O(n^2)
	if (l < 1 || l > p->col_size + 1)
		return FALSE;
	head_ele_list* temp;
	if ((temp = (head_ele_list*)malloc(sizeof(head_ele_list) * (p->col_size + 2))) == NULL)
		return FALSE;
	int i;
	for (i = 0; i < l; ++i) {
		temp[i] = p->col_head[i];
	}
	if (create_line(temp + l, p->row_size, name) == FALSE)
		return FALSE;
	for (i = l; i <= p->col_size; ++i) {
		temp[i + 1] = p->col_head[i];
	}
	p->col_head = temp;
	p->col_size++;

	for (i = 1; i <= p->row_size; ++i) {
		add_nth_ele((p->row_head + i)->head, query_ele((p->col_head + l)->head, i), l);
	}
	return TRUE;
}

char* query_row_name(Table* p, int l) {
	if (l < 1 || l > p->row_size)
		return NULL;
	return (p->row_head + l)->name;
}

char* query_col_name(Table* p, int l) {
	if (l < 1 || l > p->col_size)
		return NULL;
	return (p->col_head + l)->name;
}
