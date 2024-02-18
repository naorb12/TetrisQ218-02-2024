#ifndef _Genearal
#define _Genearal

#include <windows.h> // for gotoxy
#include <process.h> // for system
#include <iostream>
#include <conio.h>
#include "GameConfig.h"


constexpr int DOWN = 10;

void drawPoint(const int x, const int y, int backColor);
void gotoxy(const int, const int);
void clrscr();

#endif // _Genearal