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
//��������ʼλ�ã����£��������꣬���ݻ����޸�

#define color_menu_begin_x 5
#define color_menu_begin_y 1
#define color_menu_end_x 15
#define color_menu_end_y 2
//��ɫ��Ϣ����ʼλ�ã����£��������꣬���ݻ����޸�

#define dot_r 0.04
#define click_dot_r 0.05
#define array_width 0.1
#define head_height 0.4
#define head_width  1
#define pie_r 2
#define PI 3.1415926
#define color_box 0.2

#define min_height 0.05 //��ֵֹΪ0�ĵ㲻�ܱ�ѡ��

#define head_color "black"
#define background_color "HummingBird"
#define backgraph_color "white"

//extern ele* Mytable;

double draw_begin_x;
double draw_begin_y;
//��ͼ��㣬ͼ�����Ҫ���ƾ͸����

double large_rate_x;
double large_rate_y;
/*
    x,y�Ŵ���ʣ�����
    large_rate_x=2;
    ��ʾÿ��x�����Ϊԭ������
*/

bool show_axis;
bool show_pie;


/*�Ƿ�չʾ
    axis  ������
    pie   ��״ͼ
    bar   �ڼ������ݵ���״ͼ
    line  �ڼ������ݵ�����ͼ
    dot   �ڼ������ݵ�ɢ��ͼ
*/

//simulation


//simulation

//��ȫ�ֱ������˺����ڲ�ȫ�ֱ���
double width_x;//ÿһ������Ŀ��
double each_bar_width;//ÿ����������һ����״ͼ���
double last_mid_x;//ǰһ���������м��x���꣬���ڻ�������
double last_x;
int draw_pie_num;
double draw_large_rate_y;

void start_draw();//main����
void init_draw();//��ʼ��+simulation
void draw_again();//���»�һ������
void draw_bar(int num);//����״ͼ
void draw_line(int num);
void draw_dot(int num);
void draw_pie(int num);
void draw_axis();
void draw_blank();
void draw_color_menu();//���������·�����ɫ
void draw_text(int num);//��ʾx������

void init_color();

void draw_back();
void draw_axis_back();

double sum_of_col(int num);//��������״ͼ�ģ��������壬Ϊ�������
//ele* click(double* a, double* b);
double find_y_max();//����ͼ����y���ֵ����������ʾ���ģ����ص�����ʵ���ݴ�С
double find_y_min();//����ͼ����y��Сֵ����������ʾ���ģ����ص�����ʵ���ݴ�С
int show_row_num();//��ʾ��״ͼ����ʾ������������������״ͼ���

#endif 