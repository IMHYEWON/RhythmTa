#pragma once

#include<time.h>
#include<string>
#include"fmod.hpp"

using namespace std;
using namespace FMOD;

//**********************************************상수 선언**********************************************
// 키의 종류 개수
#define NumOfKey 10
//노트 개수
#define ALLNOTE 1300
// 입력 키 종류를 상수로 선언
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
// 방향키 상수로 선언
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

//*********************************************구조체&구조체 변수 선언**********************************************
// 입력 키(inputKey), 노트 종류(nKey) 구조체 선언
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KEYNOTE;
KEYNOTE KeyIndex[NumOfKey]; //입력 키&노트 종류 개수만큼 구조체 배열 선언

// 스테이지 구성
typedef enum _STAGE {
	READY, RUNNING, PAUSE, RESULT, SYNC, END
}STAGE;
STAGE Stage; 

// 시간 컨트롤러
typedef struct  CONTROL {
	clock_t MovTime; // 싱크 맞추는 이동시간
	clock_t OldTime; // 이전 이동시간
	int nMagic; // 싱크 조율 변수
}CONTROL;
CONTROL Control;

//*********************************************전역 변수 선언**********************************************
// 사운드 변수
System* pSystem;
Sound* pSound[2];
Channel* pChannel[1];

//시간
clock_t RunningTime;
clock_t PauseStart;
clock_t PauseEnd;
clock_t PauseTime = 0;
clock_t SyncStart;
clock_t SyncEnd;
clock_t SyncTime = 0;
clock_t Oldtime1 = 0;
clock_t Oldtime = 0;

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

// 노트 배열
string Note[ALLNOTE];

// 현재 perfect 구간의 Note 배열 인덱스 변수
int curNoteIndex;

// 점수 변수
int nScore; //
char strScore[20] = "  ";

// 콤보 변수
int nCombo = 0;

// 싱크 변수
int Syncnum = 1;

//*********************************************함수 선언**********************************************
//키
int isTwoKey(string note);
void CheckKey(string inputKey);
string secondkbhit(int nKey, string inputKeyStr);
string GetKeyType(string nKey);

//노트
void NoteCheck(void);
void ShowNote();
string HitNote(string inputKey);

//사운드
void SoundSystem();
void Play(int Sound_num);

//맵
void Map(void);
void ScoreMap(void);
void ReadyMap();
void ReadyMap1();
void ResultMap();
void SyncMap();

//초기화
void init();
void KeyIndexInit();

//기본 기능
void Update();
void Render(int nKey);
void Release();
