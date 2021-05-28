#include "FileControl.h"
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include "Table.h"
#include <windows.h>
#include <string.h>

static void Set_Console_Pattern1() {
    HANDLE hOut;
    //  获取输出流的句柄
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize;
    bufferSize.X = 15;
    bufferSize.Y = 10;
    SetConsoleScreenBufferSize(hOut, bufferSize);
    TCHAR title[256];
    HWND hWnd;
    GetConsoleTitle(title, 256);
    hWnd = FindWindow(0, title);
    MoveWindow(hWnd, 20, 26, 345, 300, 1);

    SetConsoleTextAttribute(hOut,
        FOREGROUND_BLUE |
        FOREGROUND_RED |     // 前景色_紫色
        FOREGROUND_INTENSITY); // 前景色_加强
}
static void Set_Console_Pattern2() {
    HANDLE hOut;
    //  获取输出流的句柄
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize;
    bufferSize.X = 15;
    bufferSize.Y = 10;
    SetConsoleScreenBufferSize(hOut, bufferSize);
    TCHAR title[256];
    HWND hWnd;
    GetConsoleTitle(title, 256);
    hWnd = FindWindow(0, title);
    MoveWindow(hWnd, 20, 26, 345, 300, 1);
    SetConsoleTextAttribute(hOut,
        FOREGROUND_BLUE |
        FOREGROUND_GREEN |     // 前景色_青色
        FOREGROUND_INTENSITY); // 前景色_加强
}
static void Search_csv_and_print() {
    //文件存储信息结构体 
    struct _finddata_t fileinfo;
    //保存文件句柄 
    long fHandle;
    //文件数记录器
    int i = 0;
    if ((fHandle = _findfirst("*.csv", &fileinfo)) == -1L)
    {
        printf("当前目录下没有csv文件\n");
        return NULL;
    }
    else {
        printf("当前目录下的所有csv文件\n");
        puts("**************************************");
        do {
            i++;
            printf("%02d) %-30s %dB\n", i, fileinfo.name, fileinfo.size);
        } while (_findnext(fHandle, &fileinfo) == 0);
        puts("**************************************");
    }
    //关闭文件 
    _findclose(fHandle);

    printf("Total number of files：%d\n\n", i);
}

static char filename[100]; // 记录打开的文件的名字

FILE* Create_File() { 
    InitConsole();
    
    Set_Console_Pattern1();

    char path[100];

    printf("请输入你想要创建的文件名:（无需后缀名）\n");
    scanf("%s", path);
    strcat(path, ".csv");
    FILE* fp;
    
    while (fp = fopen(path, "r") != NULL) {
        printf("文件名已存在！\n");
        scanf("%s", path);
        strcat(path, ".csv");
    }

    if ((fp = fopen(path, "w+")) != NULL) {
        strcpy(filename, path);
        printf("open file successful!");
    }
    FreeConsole();
    return fp;
}
FILE* Open_File() {
    InitConsole();
    Set_Console_Pattern1();
        
    Search_csv_and_print();
    
    Set_Console_Pattern2();

    printf("选择你要打开的文件\n");
    char path[100];
    scanf("%s", path);
    FILE* fp;
    if ((fp = fopen(path, "r+")) != NULL) {
        strcpy(filename, path);
        printf("open file successful!");
    }
    while (fp == NULL) {
        puts("Fail!");
        scanf("%s", path);;
        if ((fp = fopen(path, "r+")) != NULL) {
            strcpy(filename, path);
            printf("open file successful!");
        }
    }
    FreeConsole();
    return fp;
}
bool Save_File(FILE* fp, Table* p) {
    if (fclose(fp) == EOF) return FALSE;
    if ((fp = fopen(filename, "w")) == NULL)
        return FALSE;
    bool flag = Table_output(p, fp);
    if (fclose(fp) == EOF) return FALSE;
    if (fp = fopen(filename, "r+") == NULL)
        return FALSE;
    return flag;
}
bool Save_File_as(FILE **FP, Table* p) {
    // 因为文件并未被修改,所以不需要保存原来的文件  

    InitConsole();
    Set_Console_Pattern1();

    fclose(*FP);
    printf("请输入您想另存为的文件名\n");
    char path[100];
    scanf("%s", path);
    strcat(path, ".csv");
    FILE* fp;
    while (fp = fopen(path, "r") != NULL) {
        printf("文件名已存在！\n");
        scanf("%s", path);
        strcat(path, ".csv");
    }

    if ((fp = fopen(path, "w+")) != NULL) {
        printf("save file successful!");
    }
    //再次打开原来的文件
    FreeConsole();
    if (fp == NULL) {
        return FALSE;
    }
    Table_output(p, fp);
    fclose(fp);
    if ((*FP = fopen(filename, "r+")) == NULL) {
        return FALSE;
    }
    return TRUE;
}
bool Close_File(FILE * fp) {
    if (fclose(fp) != EOF) {
        memset(filename, 0, sizeof filename);
        return TRUE;
    }
    return FALSE;
}