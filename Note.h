#pragma once
#include<string>
using namespace std;
// 노트에 해당하는 변수 선언
#define NumOfKey 10
#define none 0
#define a 1
#define s 2
#define d 3
#define j 4
#define k 5
#define l 6
#define aj 7
#define sk 8
#define dl 9
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KeyNote;
KeyNote KeyIndex[NumOfKey]; //노트 개수만큼 구조체 선언

string nKeyNone = "                                      ";
string nKeyL = "                                ■■■";
string nKeyK = "                          ■■■";
string nKeyJ = "                    ■■■";
string nKeyD = "            ■■■";
string nKeyS = "      ■■■";
string nKeyA = "■■■";
string nKeyAJ = "■■■              ■■■";
string nKeySK = "      ■■■              ■■■";
string nKeyDL = "            ■■■              ■■■";