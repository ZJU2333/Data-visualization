#include "graphics.h"

#include "genlib.h"
#include "imgui.h"
#include <stdlib.h>
#include <math.h>

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"

#ifndef DRAW_H
#define DRAW_H

#define axis_begin_x 5
#define axis_begin_y 3
#define axis_end_x 15
#define axis_end_y 8
//坐标轴起始位置，左下，右上坐标，数据还需修改

#define color_menu_begin_x 5
#define color_menu_begin_y 1
#define color_menu_end_x 15
#define color_menu_end_y 2
//颜色信息栏起始位置，左下，右上坐标，数据还需修改

#define dot_r 0.04
#define click_dot_r 0.05
#define array_width 0.1
#define head_height 0.4
#define head_width  1
#define pie_r 2
#define PI 3.1415926
#define color_box 0.2

#define min_height 0.05 //防止值为0的点不能被选中

#define head_color "black"
#define background_color "HummingBird"
#define backgraph_color "white"

//extern ele* Mytable;

double draw_begin_x;
double draw_begin_y;
//画图起点，图像如果要左移就改这个

double large_rate_x;
double large_rate_y;
/*
    x,y放大比率，比如
    large_rate_x=2;
    表示每个x坐标变为原来两倍
*/

bool show_axis;
bool show_pie;


/*是否展示
    axis  坐标轴
    pie   饼状图
    bar   第几个数据的柱状图
    line  第几个数据的折线图
    dot   第几个数据的散点图
*/

//simulation


//simulation

//非全局变量，此函数内部全局变量
double width_x;//每一个区间的宽度
double each_bar_width;//每个区间内内一个柱状图宽度
double last_mid_x;//前一个区间里中间的x坐标，用于画点连线
double last_x;
int draw_pie_num;
double draw_large_rate_y;

void start_draw();//main（）
void init_draw();//初始化+simulation
void draw_again();//重新画一遍所有
void draw_bar(int num);//画柱状图
void draw_line(int num);
void draw_dot(int num);
void draw_pie(int num);
void draw_axis();
void draw_blank();
void draw_color_menu();//在坐标轴下方画颜色
void draw_text(int num);//显示x轴数据

void init_color();

void draw_back();
void draw_axis_back();

double sum_of_col(int num);//用来画饼状图的，先算总体，为了求比例
//ele* click(double* a, double* b);
double find_y_max();//现有图形里y最大值，必须是显示出的，返回的是真实数据大小
double find_y_min();//现有图形里y最小值，必须是显示出的，返回的是真实数据大小
int show_row_num();//表示柱状图里显示出的数量，用来算柱状图宽度

#endif 