#pragma once
#include<string>
using namespace std;

// 입력 키 종류를 상수로 선언
#define NumOfKey 10
#define none 0
#define a 1
#define s 2
#define d 3
#define j 4
#define k 5
#define l 6
#define aj 7
#define ks 8
#define dl 9

// 노트 종류를 string으로 선언
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

// 입력 키(inputKey)와 노트 종류(nKey)를 구조체로 선언
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KeyNote;

//입력 키&노트 종류 개수만큼 구조체 배열 선언
KeyNote KeyIndex[NumOfKey]; 