#include "Draw.h"
#include "Table.h"
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"

#define GUI_MENU
#define GUI_BUTTON
#define GUI_Graphics

extern void init_color();
bool file_open;  // �Ƿ���ļ�
FILE* file_ptr;  // �ļ�ָ��
int current_row;
int current_col;
Table* Mytable = NULL;
static double winwidth, winheight;   // ���ڳߴ�
static bool is_clear = true;
static bool flag = false;
static bool flag2 = false;
static char text[20];

void display2();
void display3();
void draw_statement();
void draw_about();
void Beautify();
void drawText();
bool to_open_file();
bool to_create_file();
bool to_close_file();
bool to_save_file();
bool to_save_file_as();
bool to_display_as_bar();
bool to_display_as_line();
bool to_display_as_dot();
void to_show_bar();
void to_show_line();
void to_show_dot();
void to_show_pie();
bool to_change_row_name();
bool to_change_col_name();
bool to_change_data_name();

// ����������provided in libgraphics
void DisplayClear(void);

// �û�����ʾ����
void display(void);

// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display(); //ˢ����ʾ
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event); // GUI��ȡ����
	display(); // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{	
	double px, py;
	uiGetMouse(x, y, button, event); //GUI��ȡ���
	display(); // ˢ����ʾ
	display2();
	display3();
	if (Mytable == NULL)// δ���ļ�ʱ��ִ��
		return;
	switch (event) {
	case BUTTON_DOWN:
		px = ScaleXInches(x);
		py = ScaleYInches(y);
		if (button == LEFT_BUTTON) {
			double a, b;
			a = px, b = py;
			if (click(&a, &b) == TRUE) {
				current_row = (int)b;
				current_col = (int)a;
				is_clear = true; 
				display();
				drawText();
				draw_again();
				is_clear = false;
				/*freopen("test.txt", "w", stdout);
				printf("%d %d\n", current_row, current_col);
				exit(0);*/
			}
		}
	}
}


// �û����������
// ����ʼ��ִ��һ��
void CreateGUI()
{
	// ��ʼ�����ں�ͼ��ϵͳ
	SetWindowTitle("���ݿ��ӻ�����");
	// ��ô��ڳߴ�
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();

	registerCharEvent(CharEventProcess); // �ַ�
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	Beautify();
	init_color();
	memset(text, 0, sizeof(text));
}

void Beautify()
{
	/*��ɫϵ*/
	DefineColor("CuriousBlue",0.20,0.59,0.86);
	DefineColor("HummingBird", 0.77, 0.94, 0.97);
	DefineColor("AliceBlue", 0.89, 0.945, 0.996);
	DefineColor("IrisBlue", 0.0, 0.71, 0.8);
	DefineColor("DodgerBlue",0.13,0.654,0.94);
	/*��ɫϵ*/
	DefineColor("NYPink", 0.88, 0.51, 0.51);
	DefineColor("SoftColor", 0.925, 0.39, 0.29);
	DefineColor("Scarlet", 0.95, 0.15, 0.075);
	DefineColor("SunsetOrange", 0.964, 0.278, 0.278);
	/*��ɫϵ*/
	DefineColor("LightWisteria", 0.745, 0.564, 0.83);
	DefineColor("ElectricPurple", 0.647, 0.215, 0.992);
	/*��ɫϵ*/
	DefineColor("BrightTurquoise", 0.16, 0.945, 0.764);
	DefineColor("LightGreen", 0.48, 0.937, 0.70);
	DefineColor("Gossip", 0.53, 0.83, 0.486);
	/*��ɫϵ*/
	DefineColor("CapeHoney", 0.992, 0.929, 0.654);
	DefineColor("TahitiGold", 0.91, 0.494, 0.015);
	DefineColor("Turbo", 0.96, 0.90, 0.105);
	DefineColor("MoonGlow", 0.996, 0.98, 0.83);
}

//#if defined(GUI_MENU)
void drawMenu()
{
	static char* menuListFile[] = { "     �ļ�",
		"�½� | Ctrl-N ", 
		"�� | Ctrl-O",
		"�ر�",
		"���� | Ctrl-S ",
		"���Ϊ",
		"�˳� | Ctrl-E " };
	static char* menuListEdit[] = { "     �༭",
		"������ʾ",
		"������ʾ",
		"������ʾ",
		"�޸�����",
		"�޸�����",
		"�޸�������"
	};
	static char* menuListDraw[] = { "     ��ͼ",
		"��ͼ",
		"����ͼ",
		"��״ͼ",
		"��״ͼ"
	};
	static char* menuListHelp[] = { "     ����",
		"ʹ��˵��",
		"����" };

	/*���ò˵������ʽ*/
	SetFont("����");
	SetPointSize(0.5);
	/*
	ʹ��DefineColor()�Զ�����ɫ
	�˵��Ͱ�ť����ɫ�޷�ͨ�����Ļ�����ɫ����
	ʹ��setMenuColors setButtonColors �޸�
	����setMenuColors����������Ϊ δ���ʱ�����ɫ ������ɫ ���������ɫ ������ɫ fillflagΪ�Ƿ���䱳��
	*/

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH * 1.5; // �ؼ��߶�
	double wlist = TextStringWidth(menuListFile[6]) * 1.2;
	double w = wlist;
	double xindent = winheight / 20; // ����
	int    selection;
	
	/*���˵���*/
	drawMenuBar(0, y - h, winwidth, h);
	/*�޸Ĳ˵���ɫ*/
	setMenuColors("AliceBlue", "DodgerBlue", "NYPink", "Scarlet", 1);


	/*���ļ��˵�*/
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); to_create_file();/*�½��ļ�����*/ is_clear = false; break;
	case 2: is_clear = true; display(); to_open_file();/*���ļ�����*/ is_clear = false; break;
	case 3:	is_clear = true; display(); to_close_file(); /*�ر��ļ�����*/ is_clear = false; break;
	case 4: is_clear = true; display(); to_save_file();/*�����ļ�����*/ is_clear = false; break;
	case 5: is_clear = true; display(); to_save_file_as();/*���Ϊ�ļ�����*/ is_clear = false; break;
	case 6: exit(-1);  break;
	}
	/*���༭�˵�*/
	selection = menuList(GenUIID(0), x+w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); to_display_as_line();/*������ʾ����*/ is_clear = false;  break;
	case 2: is_clear = true; display(); to_display_as_dot();/*������ʾ����*/ is_clear = false;  break;
	case 3: is_clear = true; display(); to_display_as_bar();/*������ʾ����*/ is_clear = false;  break;
	case 4: is_clear = true; display(); to_change_row_name();/*�����޸ĺ���*/ is_clear = false;  break;
	case 5: is_clear = true; display(); to_change_col_name();/*�����޸ĺ���*/ is_clear = false;  break;
	case 6: is_clear = true; display(); to_change_data_name();/*�������޸ĺ���*/ is_clear = false;  break;
	}
	/*����ͼ�˵�*/
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListDraw, sizeof(menuListDraw) / sizeof(menuListDraw[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); to_show_dot();/*���Ƶ�ͼ����*/ is_clear = false; break;
	case 2: is_clear = true; display(); to_show_line();/*��������ͼ����*/ is_clear = false;  break;
	case 3: is_clear = true; display(); to_show_bar();/*������״ͼ����*/ is_clear = false;  break;
	case 4: is_clear = true; display(); to_show_pie();/*���Ʊ�״ͼ����*/ is_clear = false;  break;
	}
	/*�������˵�*/
	selection = menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); flag = true; draw_statement(); /*ʹ��˵������*/ is_clear = false;  break;
	case 2: is_clear = true; display(); flag2 = true;  draw_about(); /*���ں���*/ is_clear = false;  break;
	}

}
//#endif 

//#if defined(GUI_BUTTON)
// ��ť��ʾ����
void drawButtons()
{
	SetPointSize(0.3);
	double fH = GetFontHeight();

	SetPenColor("SunsetOrange");
	MovePen(0.5, 8.0+GetFontAscent());
	DrawTextString("�鿴");
	MovePen(0.5, 8.0);
	DrawLine(3.0, 0);

	/*�޸İ�ť��ɫ*/
	setButtonColors("AliceBlue", "DodgerBlue", "NYPink", "Scarlet", 1);
	double h = fH * 2;  // �ؼ��߶�
	double x = 0.5;
	double y = 7.1;
	double w = 1.0; // �ؼ����
	if (button(GenUIID(0), x, y, w, h, "�Ŵ�X"))
	{
		/*�Ŵ�X�ĺ���*/
		is_clear = true; display();
		if (large_rate_x <= 3) large_rate_x += 0.1;
		draw_again();
		is_clear = false;
	}

	y = 6.3;
	if (button(GenUIID(0), x, y, w, h, "�Ŵ�Y"))
	{
		/*�Ŵ�Y�ĺ���*/
		is_clear = true; display();
		if (large_rate_y <= 3) large_rate_y += 0.1;
		draw_again();
		is_clear = false;
	}

	y = 5.5;
	if (button(GenUIID(0), x, y, w, h, "����"))
	{
		/*���Ƶĺ���*/
		is_clear = true; display();
		double sum = width_x * query_col_num(Mytable);
		//draw_begin_x -= 0.2;
		if (draw_begin_x > axis_begin_x )draw_begin_x -= width_x;
		draw_again();
		is_clear = false;
	}

	y = 4.7;
	if (button(GenUIID(0), x, y, w, h, "���Ƶ�ͷ"))
	{
		/*���Ƶ�ͷ�ĺ���*/
		is_clear = true; display();
		draw_begin_x = axis_begin_x;
		draw_again();
		is_clear = false;
	}

	x = 2.5;
	y = 7.1;
	if (button(GenUIID(0), x, y, w, h, "��СX"))
	{
		is_clear = true; display();
		/*��СX�ĺ���*/
		if (large_rate_x >= 0.2) large_rate_x -= 0.1;
		if(width_x / large_rate_x )
		draw_again();
		is_clear = false;
	}

	y = 6.3;
	if (button(GenUIID(0), x, y, w, h, "��СY"))
	{
		is_clear = true; display();
		/*��СY�ĺ���*/
		if (large_rate_y >=0.3)large_rate_y -= 0.1;
		draw_again();
		is_clear = false;
	}

	y = 5.5;
	if (button(GenUIID(0), x, y, w, h, "����"))
	{
		is_clear = true; display();
		/*���Ƶĺ���*/
		//draw_begin_x += 0.2;
		if(draw_begin_x + width_x * query_col_num(Mytable)< axis_end_x)draw_begin_x += width_x;
		draw_again();
		is_clear = false;
	}

	y = 4.7;
	if (button(GenUIID(0), x, y, w, h, "���Ƶ�ͷ"))
	{
		/*���Ƶ�ͷ�ĺ���*/
		is_clear = true; display();
		double sum = width_x * query_col_num(Mytable);
		draw_begin_x = (axis_end_x - sum);
		draw_again();
		is_clear = false;
	}
	SetPenColor("SunsetOrange");
	MovePen(0.5, 4.0 + GetFontAscent());
	DrawTextString("�༭");
	MovePen(0.5, 4.0);
	DrawLine(3.0, 0);

	x = 0.5;
	y = 3.1;
	if (button(GenUIID(0), x, y, w, h, "�½���"))
	{
		is_clear = true; display();

		InitConsole();
		printf("��������ݵ�����: ");
		char buf[20];
		scanf("%s", buf);
		table_add_row(Mytable, current_row, buf);
		freopen("test.txt", "w", stdout);
		
		int i, j;
		for (i = 1; i <= query_row_num(Mytable); ++i) {
			for (j = 1; j <= query_col_num(Mytable); ++j) {
				printf("%f ", table(Mytable, i, j)->num);
			}
			puts("");
		}
		printf("%p, ::%d %d\n", Mytable, query_col_num(Mytable), query_row_num(Mytable));
		//exit(0);
		/*�½��еĺ���*/
		FreeConsole();
		draw_again();
		
		is_clear = false;
	}

	y = 2.3;
	if (button(GenUIID(0), x, y, w, h, "�½���"))
	{
		is_clear = true; display();
		InitConsole();
		printf("��������ݵ�����: ");
		char buf[20];
		scanf("%s", buf);
		table_add_col(Mytable, current_row, buf);
		FreeConsole();
		draw_again();
		/*�½��еĺ���*/
		is_clear = false;
	}

	y = 1.5;
	if (button(GenUIID(0), x, y, w, h, "��ƽ��"))
	{
		is_clear = true; display();
		int i, mx = query_col_num(Mytable);
		double* temp = (double*)malloc(sizeof(double) * (mx + 1));
		for (i = 1; i <= mx; ++i) {
			temp[i] = query_col_average(Mytable, i);
		}
		table_add_row(Mytable, query_row_num(Mytable), "average");
		int j = query_row_num(Mytable);
		for (i = 1; i <= mx; ++i) {
			table(Mytable, j, i)->num = temp[i];
		}

		draw_again();/*��ƽ���ĺ���*/
		is_clear = false;
	}

	y = 0.7;
	if (button(GenUIID(0), x, y, w, h, "ѡ��һ��"))
	{
		is_clear = true; display();

		if (current_col == query_col_num(Mytable));
		else current_col++;
		draw_again();/*ѡ����һ��*/
		is_clear = false;
	}

	x = 2.5;
	y = 3.1;
	if (button(GenUIID(0), x, y, w, h, "ɾ����"))
	{
		is_clear = true; display();
		table_del_row(Mytable, current_row);
		if (query_row_num(Mytable) == 0) {
			current_row = 0;
			current_col = 0;
		}
		else if (current_row <= query_row_num(Mytable)); // Ĭ�ϲ���
		else current_row--;
		draw_again();
		/*ɾ���еĺ���*/
		is_clear = false;
	}

	y = 2.3;
	if (button(GenUIID(0), x, y, w, h, "ɾ����"))
	{
		is_clear = true; display();
		table_del_col(Mytable, current_col);
		if (query_col_num(Mytable) == 0) {
			current_row = 0;
			current_col = 0;
		}
		else if (current_col <= query_col_num(Mytable)); // Ĭ�ϲ���
		else current_col--;
		draw_again();
		/*ɾ���еĺ���*/
		is_clear = false;
	}

	y = 1.5;
	if (button(GenUIID(0), x, y, w, h, "��Ӧ����"))
	{
		is_clear = true; display();
		large_rate_y = 1;
		draw_again();/*ѡ����һ��*/
		is_clear = false;
	}

	y = 0.7;
	if (button(GenUIID(0), x, y, w, h, "ѡ��һ��"))
	{
		is_clear = true; display();

		if (current_row == 1);
		else current_row--;
		draw_again();/*ѡ����һ��*/
		is_clear = false;
	}
}
//#endif 

void drawText()
{
	char s[50] = "��ѡ�е㣺��  �� �� ��  ��";
	/*�޸Ŀճ���������
	s[12] = '1';
	s[13] = '2';
	s[22] = '3';
	s[23] = '4';
	*/
	int prex = current_row;
	int prey = current_col;
	if (prex < 10)
	{
		s[12] = '0';
		s[13] = (char)(prex + 48);
	}
	else
	{
		s[12] = (char)(prex / 10 + 48);
		s[13] = (char)(prex % 10 + 48);
	}
	if (prey < 10)
	{
		s[22] = '0';
		s[23] = (char)(prey + 48);
	}
	else
	{
		s[22] = (char)(prey / 10 + 48);
		s[23] = (char)(prey % 10 + 48);
	}
	char *color;
	color = GetPenColor();
	SetPenColor("AliceBlue");
	drawBox(4.7, 2.2, 10.6, 0.3, 1, s, 'L', "Scarlet");
	SetPenColor(color);
	if (Mytable) {
		if (current_col && current_row)
			strcpy(text, table(Mytable, current_row, current_col)->num_string);
		else {
			sprintf(text, "NULL");
			return;
		}
	}
	if (textbox(GenUIID(0),9.0,2.2,5.0,0.3,text,20))
	{
		//printf("%s\n", text);
		if (Mytable && current_col && current_row) {
			table(Mytable, current_row, current_col)->num = atof(text);
			strcpy(table(Mytable, current_row, current_col)->num_string, text);
			
			is_clear = true; display();
			draw_again(); 
			is_clear = false;
		}
	}
}

//#if defined(GUI_Graphics)
void drawGraphics()
{
	SetPenColor("HummingBird");
	drawRectangle(4.7, 2.5, 10.6, 6.0, 1);
}
//#endif

void display()
{
	// ����
	if (is_clear)
		DisplayClear();

//#if defined(GUI_Graphics)
	if (is_clear)
		drawGraphics();
//#endif

//#if defined(GUI_BUTTON)
	// ��ť
	SetPenColor("White");
	drawRectangle(0.5, 8.0, 3.0, 1.0, 1);
	drawRectangle(0.5, 4.0, 3.0, 1.0, 1);
	drawButtons();
//#endif
	drawText();
//#if defined(GUI_MENU)
	// ���ƺʹ���˵�
	//if (is_clear)
		drawMenu();
//#endif

}

void clear_open_status() {
	file_ptr = NULL;
	file_open = FALSE;
	Mytable = NULL;
	current_col = 0;
	current_row = 0;
}

bool to_open_file() {
	if (file_open)
		return FALSE;
	else {
		start_draw();
		file_ptr = Open_File();
		file_open = TRUE;
		Mytable = Generate_Table();
		bool flag = Table_input(Mytable, file_ptr);
		draw_again();
		return flag;
	}
}

bool to_create_file() {
	if (file_open)
		return FALSE;
	else {
		start_draw();
		file_ptr = Create_File();
		file_open = TRUE;
		Mytable = Generate_Table();
		draw_again();
		return TRUE;
	}
}

bool to_close_file() {
	if (!file_open) {
		return FALSE;
	}
	else {
		Close_File(file_ptr);
		clear_open_status();
		drawText();
		draw_again();
		return TRUE;
	}
}
bool to_save_file() {
	bool flag = Save_File(file_ptr, Mytable);
	draw_again();
	return flag;
}

bool to_save_file_as() {
	bool flag = Save_File_as(&file_ptr, Mytable);
	draw_again();
	return flag;
}

bool to_display_as_bar() {
	bool rt = let_it_show_bar(Mytable, current_row);
	draw_again();
	return rt;
}

bool to_display_as_line() {
	bool rt = let_it_show_line(Mytable, current_row);
	draw_again();
	return rt;
}

bool to_display_as_dot() {
	bool rt = let_it_show_dot(Mytable, current_row);
	draw_again();
	return rt;
}

void to_show_bar() {
	show_axis = TRUE;
	show_pie = FALSE;
	table_show_bar(Mytable);
	draw_again();
}

void to_show_line() {
	show_axis = TRUE;
	show_pie = FALSE;
	table_show_line(Mytable);
	draw_again();
}

void to_show_dot() {
	show_axis = TRUE;
	show_pie = FALSE;
	table_show_dot(Mytable);
	draw_again();
}

void to_show_pie() {
	show_axis = FALSE;
	show_pie = TRUE;
	draw_again();
}

bool to_change_row_name() {
	InitConsole();
	printf("��������Ҫ������: ");
	char buf[20];
	scanf("%s", buf);
	bool flag = change_row_name(Mytable, current_row, buf);
	FreeConsole();
	draw_again();
	return flag;
}

bool to_change_col_name() {
	InitConsole();
	printf("��������Ҫ������: ");
	char buf[20];
	scanf("%s", buf);
	bool flag = change_col_name(Mytable, current_col, buf);
	FreeConsole();
	draw_again();
	return flag;
}
bool to_change_data_name() {
	InitConsole();
	if (Mytable) {
		printf("ԭ��������: ");
		if (Mytable->data_name) {
			printf("%s", Mytable->data_name);
		}
		putchar('\n');
	}
	printf("��������Ҫ������: ");
	char buf[20];
	scanf("%s", buf);
	bool flag = change_data_name(Mytable, buf);
	FreeConsole();
	draw_again();
	return flag;
}

void draw_statement() {
	mySetPenColor("white");
	drawRectangle(0, 0, 16, 9, 1);
	mySetPenColor("HummingBird");
	drawRectangle(2, 0.5, 12, 8, 1);
	double height = 0.4, head = 7.2;
	int hang = 1;
	SetFont("����");
	mySetPenColor("black");
	SetPointSize(0.9);
	MovePen(7, 7.5);
	DrawTextString("ʹ��˵��");
	SetPointSize(0.5);
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(1) ����������");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("��ʼ�����Ϊ�˵�������ͼ���ͱ༭��");
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(2) ��ͼ������");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("����ͼ��Ϊɢ��ͼ������ͼ����״ͼ����ͳ��ͼʾ�����ÿһ����");
	MovePen(3, head - (hang++) * height);
	DrawTextString("�ݿ�ѡ��ʹ����ʾ����ͼ������һ�֡���ѡ��ĳһ��������ʾ��״ͼ��");
	MovePen(3, head - (hang++) * height);
	DrawTextString("����޸ģ��ɵ����һ��/��/����������ʾ�������޸������ݡ�");
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(3) �༭������");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("��ѡ��Ŵ�x��,y������������ƶ�ͼ����в鿴ͼ��ı䶯����");
	MovePen(3, head - (hang++) * height);
	DrawTextString("����ť������޸�ѡ�е���Ϣ�������С������ݣ������С������ݵȲ�");
	MovePen(3, head - (hang++) * height);
	DrawTextString("����");
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(4) �ļ�������");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("��ͨ��csv��ʽ����excel����ڴ˳��������ͳ�����ݿ��ӻ�");
	MovePen(3, head - (hang++) * height);
	DrawTextString("�ڿ��ӻ���Ҳ������������ݡ��޸����ݵĲ������ڳ������н���ǰ");
	MovePen(3, head - (hang++) * height);
	DrawTextString("Ӧȷ���޸������ѱ��档");
	/*��һ��������ҳ�İ�ť*/
	if (button(GenUIID(0), 7.0, 0.8, 2.0, 0.5, "����"))
	{
		flag = false;
		is_clear = true;
		display();

	}
}

void draw_about()
{
	mySetPenColor("white");
	drawRectangle(0, 0, 16, 9, 1);
	mySetPenColor("HummingBird");
	drawRectangle(2, 0.5, 12, 8, 1);
	SetFont("����");
	SetPenSize(10.0);
	mySetPenColor("black");
	MovePen(7.5, 6.0);
	DrawTextString("���ڱ���");
	MovePen(7.2, 5.5);
	DrawTextString("������ZJU2333��");
	MovePen(6.6, 5.0);
	DrawTextString("��Ա�� Ҷ�󿭡���Ң�����γ�");
	SetPenSize(1.0);
	if (button(GenUIID(0), 7.0, 0.8, 2.0, 0.5, "����"))
	{
		flag2 = false;
		is_clear = true;
		display();
	}
}

void display2()
{
	if (flag) draw_statement();
}

void display3()
{
	if (flag2) draw_about();
}