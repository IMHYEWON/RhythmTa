// 노트 변수 모듈화
/*
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include"Screen.h""

#define ALLNOTE 300



char* nKeyNone = "                                      ";
char* nKeyL = "                                ■■■";
char* nKeyK = "                          ■■■";
char* nKeyJ = "                    ■■■";
char* nKeyD = "            ■■■";
char* nKeyS = "      ■■■";
char* nKeyA = "■■■";

// 19개 / 3 / 3 / 3 / 1 / 3 / 3 / 3
char* Note[ALLNOTE] = { NULL, };

void NoteCheck(void) {
	Note[29] = nKeyL;
	Note[30] = nKeyD;
	Note[31] = nKeyL;
	Note[32] = nKeyS;
	Note[33] = nKeyL;
	Note[34] = nKeyD;
	Note[35] = nKeyL;
	Note[36] = nKeyS;
}


void ShowNote(int n) {
	for (int i = 0; i < 28; i++) {
		ScreenPrint(2, i + 1, Note[27-i+n]);
	}
}
*/


