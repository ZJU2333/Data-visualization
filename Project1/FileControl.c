#include "FileControl.h"
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include "Table.h"
#include <windows.h>
#include <string.h>

static void Set_Console_Pattern1() {
    HANDLE hOut;
    //  ��ȡ������ľ��
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
        FOREGROUND_RED |     // ǰ��ɫ_��ɫ
        FOREGROUND_INTENSITY); // ǰ��ɫ_��ǿ
}
static void Set_Console_Pattern2() {
    HANDLE hOut;
    //  ��ȡ������ľ��
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
        FOREGROUND_GREEN |     // ǰ��ɫ_��ɫ
        FOREGROUND_INTENSITY); // ǰ��ɫ_��ǿ
}
static void Search_csv_and_print() {
    //�ļ��洢��Ϣ�ṹ�� 
    struct _finddata_t fileinfo;
    //�����ļ���� 
    long fHandle;
    //�ļ�����¼��
    int i = 0;
    if ((fHandle = _findfirst("*.csv", &fileinfo)) == -1L)
    {
        printf("��ǰĿ¼��û��csv�ļ�\n");
        return NULL;
    }
    else {
        printf("��ǰĿ¼�µ�����csv�ļ�\n");
        puts("**************************************");
        do {
            i++;
            printf("%02d) %-30s %dB\n", i, fileinfo.name, fileinfo.size);
        } while (_findnext(fHandle, &fileinfo) == 0);
        puts("**************************************");
    }
    //�ر��ļ� 
    _findclose(fHandle);

    printf("Total number of files��%d\n\n", i);
}

static char filename[100]; // ��¼�򿪵��ļ�������

FILE* Create_File() { 
    InitConsole();
    
    Set_Console_Pattern1();

    char path[100];

    printf("����������Ҫ�������ļ���:�������׺����\n");
    scanf("%s", path);
    strcat(path, ".csv");
    FILE* fp;
    
    while (fp = fopen(path, "r") != NULL) {
        printf("�ļ����Ѵ��ڣ�\n");
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

    printf("ѡ����Ҫ�򿪵��ļ�\n");
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
    // ��Ϊ�ļ���δ���޸�,���Բ���Ҫ����ԭ�����ļ�  

    InitConsole();
    Set_Console_Pattern1();

    fclose(*FP);
    printf("�������������Ϊ���ļ���\n");
    char path[100];
    scanf("%s", path);
    strcat(path, ".csv");
    FILE* fp;
    while (fp = fopen(path, "r") != NULL) {
        printf("�ļ����Ѵ��ڣ�\n");
        scanf("%s", path);
        strcat(path, ".csv");
    }

    if ((fp = fopen(path, "w+")) != NULL) {
        printf("save file successful!");
    }
    //�ٴδ�ԭ�����ļ�
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