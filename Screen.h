#pragma once

#include <string>

using namespace std;

void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();
void ScreenPrint(int x, int y, string string);
void SetColor(unsigned short color);