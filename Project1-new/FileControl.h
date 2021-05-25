#ifndef _FileControl_h_
#define _FileControl_h_
#include "genlib.h"
#include "Table.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
FILE* Open_File();//默认已经关闭了前一个文件
FILE* Create_File();//默认已经关闭了前一个文件
bool Close_File();
bool Save_File(FILE* fp, Table* p);
bool Save_File_as(Table* p);
#endif