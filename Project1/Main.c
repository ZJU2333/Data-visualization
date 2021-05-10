#include "Table.h"
#include "Myio.h"
#include "FileControl.h"

#include <windows.h>

void TimerEventProcess(int timerID);/*定时器消息回调函数*/
void Main() {

	//FILE* fp;
	//fp = Create_File();
	//Table* p = Generate_Table();
	//File_Save_as(p);

	//FILE* fp;
	//fp = Open_File();
	//if (fp != NULL) {
	//	freopen("result.txt", "w", stdout);
	//	Table* p = Generate_Table();
	//	if (p != NULL && Table_input(p, fp)) {
	//		table_add_row(p, 2, "Test");
	//		table_del_col(p, 2);
	//		Save_File_as(p);
	//	}
	//}

    //InitGraphics();





	//char s[100];
	//scanf("%s", s);
	//FreeConsole();


	//registerTimerEvent(TimerEventProcess);/*注册定时器消息回调函数*/
	//return;
}
void TimerEventProcess(int timerID) {
}