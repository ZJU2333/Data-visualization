#include "Myio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 默认最大数据范围为20*20 */
static int Max(int a, int b) {
	return b < a ? a : b;
}
bool Table_input(Table* p, FILE* fp) { //O(n^3)
	
	char buf[21][21][10], ch;
	int index = 0, i = 0, j = 0, col_size = 0, row_size = 0;

	while ((ch = fgetc(fp)) != EOF) {
		row_size = Max(row_size, i);
		col_size = Max(col_size, j);

		switch (ch) {
		case ',':
			buf[i][j][index] = 0;
			index = 0;
			j++;
			break;
		case '\n':
			buf[i][j][index] = 0;
			index = 0;
			i++, j = 0;
			break;
		default:
			buf[i][j][index++] = ch;
			break;
		}
	}

	//利用原有的功能实现赋值，因为实在懒得再写一个函数了
	p->data_name = (char*)malloc(sizeof(buf[0][0]));
	strcpy(p->data_name, buf[0][0]);
	
	p->row_size = row_size;
	p->row_head = (head_ele_list*)malloc(sizeof(head_ele_list) * (p->row_size + 1));
	
	//printf("%d\n", p->row_size);
	for (i = 1; i <= p->row_size; ++i) {
		head_ele_list* hd = p->row_head + i;
		hd->head = (ele_list*)malloc(sizeof(ele_list));
		hd->name = (char*)malloc(sizeof(buf[i][0]));
		strcpy(hd->name, buf[i][0]);
		//printf("%s\n", hd->name);
	}
	//exit(0);

	for (j = 1; j <= col_size; ++j) {
		table_add_col(p, j - 1, buf[0][j]);
	}

	for (i = 1; i <= row_size; ++i) {
		for (j = 1; j <= col_size; ++j) {
			table(p, i, j)->num = atof(buf[i][j]);
			//printf("%f\n", atof(buf[i][j]));
		}
	}

	return TRUE;
}

bool Table_output(Table* p, FILE* fp) { // O(n^4)
	if (fp == NULL) return FALSE; //如果文件是空的，返回失败

	fprintf(fp, "%s,", p->data_name);

	int i, j;
	for (j = 1; j <= p->col_size; ++j) {
		fprintf(fp, "%s%c", query_col_name(p, j), (j == p->col_size) ? '\r' : ',');
		if (j == p->col_size) putchar('\n');
	}

	for (i = 1; i <= p->row_size; ++i) {
		fprintf(fp, "%s,", query_row_name(p, i));
		for (j = 1; j <= p->col_size; ++j) {
			fprintf(fp, "%.6f%c", table(p,i,j)->num, (j == p->col_size) ? '\r' : ',');
			if (j == p->col_size) putchar('\n');
		}
	}

	return TRUE;
}