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
bool file_open;  // 是否打开文件
FILE* file_ptr;  // 文件指针
int current_row;
int current_col;
Table* Mytable = NULL;
static double winwidth, winheight;   // 窗口尺寸
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

// 清屏函数，provided in libgraphics
void DisplayClear(void);

// 用户的显示函数
void display(void);

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event); // GUI获取键盘
	display(); // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{	
	double px, py;
	uiGetMouse(x, y, button, event); //GUI获取鼠标
	display(); // 刷新显示
	display2();
	display3();
	if (Mytable == NULL)// 未打开文件时不执行
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


// 用户主程序入口
// 仅初始化执行一次
void CreateGUI()
{
	// 初始化窗口和图形系统
	SetWindowTitle("数据可视化工具");
	// 获得窗口尺寸
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();

	registerCharEvent(CharEventProcess); // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	Beautify();
	init_color();
	memset(text, 0, sizeof(text));
}

void Beautify()
{
	/*蓝色系*/
	DefineColor("CuriousBlue",0.20,0.59,0.86);
	DefineColor("HummingBird", 0.77, 0.94, 0.97);
	DefineColor("AliceBlue", 0.89, 0.945, 0.996);
	DefineColor("IrisBlue", 0.0, 0.71, 0.8);
	DefineColor("DodgerBlue",0.13,0.654,0.94);
	/*红色系*/
	DefineColor("NYPink", 0.88, 0.51, 0.51);
	DefineColor("SoftColor", 0.925, 0.39, 0.29);
	DefineColor("Scarlet", 0.95, 0.15, 0.075);
	DefineColor("SunsetOrange", 0.964, 0.278, 0.278);
	/*紫色系*/
	DefineColor("LightWisteria", 0.745, 0.564, 0.83);
	DefineColor("ElectricPurple", 0.647, 0.215, 0.992);
	/*绿色系*/
	DefineColor("BrightTurquoise", 0.16, 0.945, 0.764);
	DefineColor("LightGreen", 0.48, 0.937, 0.70);
	DefineColor("Gossip", 0.53, 0.83, 0.486);
	/*橘色系*/
	DefineColor("CapeHoney", 0.992, 0.929, 0.654);
	DefineColor("TahitiGold", 0.91, 0.494, 0.015);
	DefineColor("Turbo", 0.96, 0.90, 0.105);
	DefineColor("MoonGlow", 0.996, 0.98, 0.83);
}

//#if defined(GUI_MENU)
void drawMenu()
{
	static char* menuListFile[] = { "     文件",
		"新建 | Ctrl-N ", 
		"打开 | Ctrl-O",
		"关闭",
		"保存 | Ctrl-S ",
		"另存为",
		"退出 | Ctrl-E " };
	static char* menuListEdit[] = { "     编辑",
		"折线显示",
		"点阵显示",
		"柱形显示",
		"修改行名",
		"修改列名",
		"修改数据名"
	};
	static char* menuListDraw[] = { "     绘图",
		"点图",
		"折线图",
		"柱状图",
		"饼状图"
	};
	static char* menuListHelp[] = { "     帮助",
		"使用说明",
		"关于" };

	/*设置菜单字体格式*/
	SetFont("仿宋");
	SetPointSize(0.5);
	/*
	使用DefineColor()自定义颜色
	菜单和按钮的颜色无法通过更改画笔颜色更改
	使用setMenuColors setButtonColors 修改
	其中setMenuColors从左到右依次为 未点击时框架颜色 文字颜色 点击后框架颜色 文字颜色 fillflag为是否填充背景
	*/

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH * 1.5; // 控件高度
	double wlist = TextStringWidth(menuListFile[6]) * 1.2;
	double w = wlist;
	double xindent = winheight / 20; // 缩进
	int    selection;
	
	/*画菜单栏*/
	drawMenuBar(0, y - h, winwidth, h);
	/*修改菜单颜色*/
	setMenuColors("AliceBlue", "DodgerBlue", "NYPink", "Scarlet", 1);


	/*画文件菜单*/
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); to_create_file();/*新建文件函数*/ is_clear = false; break;
	case 2: is_clear = true; display(); to_open_file();/*打开文件函数*/ is_clear = false; break;
	case 3:	is_clear = true; display(); to_close_file(); /*关闭文件函数*/ is_clear = false; break;
	case 4: is_clear = true; display(); to_save_file();/*保存文件函数*/ is_clear = false; break;
	case 5: is_clear = true; display(); to_save_file_as();/*另存为文件函数*/ is_clear = false; break;
	case 6: exit(-1);  break;
	}
	/*画编辑菜单*/
	selection = menuList(GenUIID(0), x+w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); to_display_as_line();/*折线显示函数*/ is_clear = false;  break;
	case 2: is_clear = true; display(); to_display_as_dot();/*点阵显示函数*/ is_clear = false;  break;
	case 3: is_clear = true; display(); to_display_as_bar();/*柱形显示函数*/ is_clear = false;  break;
	case 4: is_clear = true; display(); to_change_row_name();/*行名修改函数*/ is_clear = false;  break;
	case 5: is_clear = true; display(); to_change_col_name();/*列名修改函数*/ is_clear = false;  break;
	case 6: is_clear = true; display(); to_change_data_name();/*数据名修改函数*/ is_clear = false;  break;
	}
	/*画绘图菜单*/
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListDraw, sizeof(menuListDraw) / sizeof(menuListDraw[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); to_show_dot();/*绘制点图函数*/ is_clear = false; break;
	case 2: is_clear = true; display(); to_show_line();/*绘制折线图函数*/ is_clear = false;  break;
	case 3: is_clear = true; display(); to_show_bar();/*绘制柱状图函数*/ is_clear = false;  break;
	case 4: is_clear = true; display(); to_show_pie();/*绘制饼状图函数*/ is_clear = false;  break;
	}
	/*画帮助菜单*/
	selection = menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	switch (selection)
	{
	case 1: is_clear = true; display(); flag = true; draw_statement(); /*使用说明函数*/ is_clear = false;  break;
	case 2: is_clear = true; display(); flag2 = true;  draw_about(); /*关于函数*/ is_clear = false;  break;
	}

}
//#endif 

//#if defined(GUI_BUTTON)
// 按钮演示程序
void drawButtons()
{
	SetPointSize(0.3);
	double fH = GetFontHeight();

	SetPenColor("SunsetOrange");
	MovePen(0.5, 8.0+GetFontAscent());
	DrawTextString("查看");
	MovePen(0.5, 8.0);
	DrawLine(3.0, 0);

	/*修改按钮颜色*/
	setButtonColors("AliceBlue", "DodgerBlue", "NYPink", "Scarlet", 1);
	double h = fH * 2;  // 控件高度
	double x = 0.5;
	double y = 7.1;
	double w = 1.0; // 控件宽度
	if (button(GenUIID(0), x, y, w, h, "放大X"))
	{
		/*放大X的函数*/
		is_clear = true; display();
		if (large_rate_x <= 3) large_rate_x += 0.1;
		draw_again();
		is_clear = false;
	}

	y = 6.3;
	if (button(GenUIID(0), x, y, w, h, "放大Y"))
	{
		/*放大Y的函数*/
		is_clear = true; display();
		if (large_rate_y <= 3) large_rate_y += 0.1;
		draw_again();
		is_clear = false;
	}

	y = 5.5;
	if (button(GenUIID(0), x, y, w, h, "左移"))
	{
		/*左移的函数*/
		is_clear = true; display();
		double sum = width_x * query_col_num(Mytable);
		//draw_begin_x -= 0.2;
		if (draw_begin_x > axis_begin_x )draw_begin_x -= width_x;
		draw_again();
		is_clear = false;
	}

	y = 4.7;
	if (button(GenUIID(0), x, y, w, h, "左移到头"))
	{
		/*左移到头的函数*/
		is_clear = true; display();
		draw_begin_x = axis_begin_x;
		draw_again();
		is_clear = false;
	}

	x = 2.5;
	y = 7.1;
	if (button(GenUIID(0), x, y, w, h, "缩小X"))
	{
		is_clear = true; display();
		/*缩小X的函数*/
		if (large_rate_x >= 0.2) large_rate_x -= 0.1;
		if(width_x / large_rate_x )
		draw_again();
		is_clear = false;
	}

	y = 6.3;
	if (button(GenUIID(0), x, y, w, h, "缩小Y"))
	{
		is_clear = true; display();
		/*缩小Y的函数*/
		if (large_rate_y >=0.3)large_rate_y -= 0.1;
		draw_again();
		is_clear = false;
	}

	y = 5.5;
	if (button(GenUIID(0), x, y, w, h, "右移"))
	{
		is_clear = true; display();
		/*右移的函数*/
		//draw_begin_x += 0.2;
		if(draw_begin_x + width_x * query_col_num(Mytable)< axis_end_x)draw_begin_x += width_x;
		draw_again();
		is_clear = false;
	}

	y = 4.7;
	if (button(GenUIID(0), x, y, w, h, "右移到头"))
	{
		/*右移到头的函数*/
		is_clear = true; display();
		double sum = width_x * query_col_num(Mytable);
		draw_begin_x = (axis_end_x - sum);
		draw_again();
		is_clear = false;
	}
	SetPenColor("SunsetOrange");
	MovePen(0.5, 4.0 + GetFontAscent());
	DrawTextString("编辑");
	MovePen(0.5, 4.0);
	DrawLine(3.0, 0);

	x = 0.5;
	y = 3.1;
	if (button(GenUIID(0), x, y, w, h, "新建行"))
	{
		is_clear = true; display();

		InitConsole();
		printf("输入该数据的名字: ");
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
		/*新建行的函数*/
		FreeConsole();
		draw_again();
		
		is_clear = false;
	}

	y = 2.3;
	if (button(GenUIID(0), x, y, w, h, "新建列"))
	{
		is_clear = true; display();
		InitConsole();
		printf("输入该数据的名字: ");
		char buf[20];
		scanf("%s", buf);
		table_add_col(Mytable, current_row, buf);
		FreeConsole();
		draw_again();
		/*新建列的函数*/
		is_clear = false;
	}

	y = 1.5;
	if (button(GenUIID(0), x, y, w, h, "求平均"))
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

		draw_again();/*求平均的函数*/
		is_clear = false;
	}

	y = 0.7;
	if (button(GenUIID(0), x, y, w, h, "选下一列"))
	{
		is_clear = true; display();

		if (current_col == query_col_num(Mytable));
		else current_col++;
		draw_again();/*选中下一列*/
		is_clear = false;
	}

	x = 2.5;
	y = 3.1;
	if (button(GenUIID(0), x, y, w, h, "删除行"))
	{
		is_clear = true; display();
		table_del_row(Mytable, current_row);
		if (query_row_num(Mytable) == 0) {
			current_row = 0;
			current_col = 0;
		}
		else if (current_row <= query_row_num(Mytable)); // 默认不动
		else current_row--;
		draw_again();
		/*删除行的函数*/
		is_clear = false;
	}

	y = 2.3;
	if (button(GenUIID(0), x, y, w, h, "删除列"))
	{
		is_clear = true; display();
		table_del_col(Mytable, current_col);
		if (query_col_num(Mytable) == 0) {
			current_row = 0;
			current_col = 0;
		}
		else if (current_col <= query_col_num(Mytable)); // 默认不动
		else current_col--;
		draw_again();
		/*删除列的函数*/
		is_clear = false;
	}

	y = 1.5;
	if (button(GenUIID(0), x, y, w, h, "适应坐标"))
	{
		is_clear = true; display();
		large_rate_y = 1;
		draw_again();/*选中下一行*/
		is_clear = false;
	}

	y = 0.7;
	if (button(GenUIID(0), x, y, w, h, "选上一行"))
	{
		is_clear = true; display();

		if (current_row == 1);
		else current_row--;
		draw_again();/*选中下一行*/
		is_clear = false;
	}
}
//#endif 

void drawText()
{
	char s[50] = "被选中点：第  行 ， 第  列";
	/*修改空出来的数字
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
	// 清屏
	if (is_clear)
		DisplayClear();

//#if defined(GUI_Graphics)
	if (is_clear)
		drawGraphics();
//#endif

//#if defined(GUI_BUTTON)
	// 按钮
	SetPenColor("White");
	drawRectangle(0.5, 8.0, 3.0, 1.0, 1);
	drawRectangle(0.5, 4.0, 3.0, 1.0, 1);
	drawButtons();
//#endif
	drawText();
//#if defined(GUI_MENU)
	// 绘制和处理菜单
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
	printf("输入你想要的名字: ");
	char buf[20];
	scanf("%s", buf);
	bool flag = change_row_name(Mytable, current_row, buf);
	FreeConsole();
	draw_again();
	return flag;
}

bool to_change_col_name() {
	InitConsole();
	printf("输入你想要的名字: ");
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
		printf("原来的名字: ");
		if (Mytable->data_name) {
			printf("%s", Mytable->data_name);
		}
		putchar('\n');
	}
	printf("输入你想要的名字: ");
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
	SetFont("仿宋");
	mySetPenColor("black");
	SetPointSize(0.9);
	MovePen(7, 7.5);
	DrawTextString("使用说明");
	SetPointSize(0.5);
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(1) 基本分区：");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("初始界面分为菜单栏，绘图区和编辑区");
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(2) 绘图操作：");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("坐标图分为散点图、折线图、柱状图三种统计图示，针对每一个数");
	MovePen(3, head - (hang++) * height);
	DrawTextString("据可选择使其显示三种图像其中一种。可选中某一列数据显示饼状图。");
	MovePen(3, head - (hang++) * height);
	DrawTextString("点击修改：可点击任一点/柱/饼，在其显示高亮后修改其数据。");
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(3) 编辑操作：");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("可选择放大x轴,y轴比例，左右移动图像进行查看图像的变动。点");
	MovePen(3, head - (hang++) * height);
	DrawTextString("击按钮可完成修改选中点信息，新增行、列数据，交换行、列数据等操");
	MovePen(3, head - (hang++) * height);
	DrawTextString("作。");
	MovePen(3, head - (hang++) * height);
	mySetPenColor("Deep4");
	DrawTextString("(4) 文件操作：");
	MovePen(3.6, head - (hang++) * height);
	mySetPenColor("Black");
	DrawTextString("可通过csv格式保存excel表格并在此程序中完成统计数据可视化");
	MovePen(3, head - (hang++) * height);
	DrawTextString("在可视化中也能完成新增数据、修改数据的操作，在程序运行结束前");
	MovePen(3, head - (hang++) * height);
	DrawTextString("应确认修改数据已保存。");
	/*差一个返回主页的按钮*/
	if (button(GenUIID(0), 7.0, 0.8, 2.0, 0.5, "返回"))
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
	SetFont("仿宋");
	SetPenSize(10.0);
	mySetPenColor("black");
	MovePen(7.5, 6.0);
	DrawTextString("关于本组");
	MovePen(7.2, 5.5);
	DrawTextString("组名：ZJU2333队");
	MovePen(6.6, 5.0);
	DrawTextString("组员： 叶泽凯、汤尧、胡嘉成");
	SetPenSize(1.0);
	if (button(GenUIID(0), 7.0, 0.8, 2.0, 0.5, "返回"))
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