#include "Draw.h"
#include "Table.h"
#include "FileControl.h"

extern void CreateGUI();

void Main() {
	SetWindowSize(16.0, 9.0);
	InitGraphics();
	CreateGUI();
	return;
}