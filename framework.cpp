#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include"Screen.h"
#include "Note.h"
#include<memory.h>
#include<conio.h>
#include<string>
#include"fmod.hpp"

using namespace std;
using namespace FMOD;

#define ALLNOTE 1000
//

System* pSystem;
Sound* pSound[2];
Channel* pChannel[1];

void SoundSystem() {
	System_Create(&pSystem);

	pSystem->init(4, FMOD_INIT_NORMAL, NULL);

	pSystem->createSound("opening.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[0]); // 배경음악
	pSystem->createSound("Festival_of_Ghost.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[1]); // 배경음악
}

void Play(int Sound_num) {

	pSystem->playSound(FMOD_CHANNEL_FREE, pSound[Sound_num], 0, pChannel);

}

//
int n = 0;

int nScore = 0;
char strScore[20] = "  ";
int nCombo = 0;
//
clock_t RunningTime;
clock_t PauseStart;
clock_t PauseEnd;
clock_t PauseTime = 0;

string checkstring;
// 노트 판별 존
typedef struct _NOTECOUNT {
	int nXofA;   //(2,29)
	int nXofS;
	int nXofD;
	int nXofJ;
	int nXofK;
	int nXofL;
}NOTECOUNT;
NOTECOUNT Count;

// 스테이지 구성
typedef enum _STAGE {
	READY, RUNNING,PAUSE, RESULT
}STAGE;
STAGE Stage;



// 스테이지 기본 틀
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "□□□□□□□□□□□□□□□□□□□□□");
	for (int i = 1; i < 29; i++) {
		ScreenPrint(0, i, "□\t\t\t\t\t□");
	}
	ScreenPrint(60, 22, checkstring);
	ScreenPrint(0, 29, "□□□□□□□□□□□□□□□□□□□□□");
	ScreenPrint(2, 26, "______________________________________");
}

// 우측 점수 출력틀
void ScoreMap() {
	// 경과시간
	char nTime[20];//경과 시간을 나타낸다
	sprintf(nTime, "시간 : %d.%d초", RunningTime / 1000, RunningTime % 1000);
	ScreenPrint(44, 2, nTime);
	// 점수 목록
	ScreenPrint(44, 10, strScore);//Great,Perfect판별
	ScreenPrint(44, 22, "Great : 300점");
	ScreenPrint(44, 23, "Perfect : 500점");
	SetColor(12);
	ScreenPrint(44, 6, "Press \'p\' to Pause");
	SetColor(15);
	// 점수
	char UserScore[20];//사용자 점수를 나타냄
	sprintf(UserScore, "점수 : %d 점", nScore);
	ScreenPrint(44, 4, UserScore);
	ScreenPrint(44, 27, "<<< 히트 구간(G)");
	ScreenPrint(44, 28, "<<< 히트 구간(P)");
	ScreenPrint(44, 29, "<<< 히트 구간(G)");
	//콤보
	char strCombo[20];//콤보를 나타낸다
	sprintf(strCombo, "%d 콤보", nCombo);
	ScreenPrint(44, 13, strCombo);
}



// 시간 컨트롤러
typedef struct  CONTROL {
	clock_t MovTime; // 싱크 맞추는 이동시간
	clock_t OldTime; // 이전 이동시간
	int nMagic; // 싱크 조율 변수

}CONTROL;
CONTROL Control;

// 게임 실행 전 준비화면
void ReadyMap() {
	ScreenPrint(15, 10, "유령의 축제");
	ScreenPrint(2, 26, "□□□■■■□□□  ■■■□□□■■■");
	ScreenPrint(2, 27, "  A     S     D       J     K      L");
	// 게임 조작 키 설명 
}

// Render함수에서 깜빡이면서 출력
void ReadyMap1() {
	SetColor(10);
	ScreenPrint(10, 15, "Press Enter to Start");
	SetColor(15);
}

void ResultMap()
{
	
	ScreenPrint(9, 7, "┌-------------------┐");
	for (int i = 8; i < 15; i++) {
		ScreenPrint(9, i, "│\t\t     │");
	}
	SetColor(9); ScreenPrint(15, 10, "GAME END !");SetColor(15);
	char UserScore[20];//사용자 점수를 나타냄
	sprintf(UserScore, "Score : %d 점", nScore);
	ScreenPrint(14, 12, UserScore);
	ScreenPrint(9, 15, "└-------------------┘");

}



// 19개 / 3 / 3 / 3 / 1 / 3 / 3 / 3
string Note[ALLNOTE];
void KeyIndexInit();
int isTwoKey(string note);

void CheckKey(string inputKey);
void NoteCheck(void);



// 2차원 배열을 아래로 떨어지게끔 해주는 함수
void ShowNote(int n) {
	for (int i = 0; i < 28; i++) {
		ScreenPrint(2, 28-i, Note[n+i]);
	}
}








void init() {
	Control.MovTime = 52;
	Control.OldTime = 0;
	Control.nMagic = 1;
	Stage = READY;
	
	for (int i = 0; i < ALLNOTE; i++) {
		Note[i] = " ";
	}
	RunningTime = 0;
	NoteCheck();
	Count.nXofA = 2;   //(2,29)
	Count.nXofS = 8;
	Count.nXofD = 14;
	Count.nXofJ = 21;
	Count.nXofK = 27;
	Count.nXofL = 33;

	//Stage = READY;

}


clock_t Oldtime = 0;
void Update() {
	clock_t Curtime = clock();
	//Control.nMagic = 1;
	switch (Stage) {
	case READY :
		Oldtime = Curtime;
		break;
	case RUNNING:
		// 게임 시작 후 시간 측정변수
		RunningTime = Curtime - Oldtime - PauseTime;
		break;
	case PAUSE:
		break;
	//case RESULT:
	//	break;
	}
	//NoteCheck();
}





clock_t Oldtime1 = 0;
void Render() {
	clock_t Curtime = clock(); // 지금까지 흐른 시간
	ScreenClear();
	//출력코드
	Map();
	ScoreMap();
	switch (Stage) {
	case READY ://대기상태
		Oldtime1 = Curtime;
		ReadyMap();
		if (Curtime % 1000 > 500) {
			ReadyMap1();
		}//0.5초 단위로 화면을 출력
		break;
	case PAUSE:
		return;
	case RUNNING :
		if (RunningTime > 3100) //3초 이후부터
		{
			if (Curtime - Control.OldTime > Control.MovTime) 
			{
				Control.OldTime = Curtime;
				n++;//노트가 저장된 배열의 인덱스를 증가
			}
			ShowNote(n);
		}
		break;
	//case RESULT:
	//	
	//	break;
	}






	ScreenFlipping();
}

void Release() {

}
string secondkbhit(int nKey, string inputKeyStr) {
	int nKey2; // 두 번째로 입력된 키를 받기 위한 변수
	for (int i = 0; i < 1000; i++) {
		if (_kbhit()) {
			nKey2 = _getch();
			if (nKey2 == 'a' || nKey2 == 's' || nKey2 == 'd' || nKey2 == 'j' || nKey2 == 'k' || nKey2 == 'l') {
				if (nKey == nKey2) { //이전 키가 길게 눌렸을 경우 인식하는 것을 방지
					continue;
				}
				else if (nKey < nKey2) { //알파벳 순서가 빠른 것을 inputKeyStr에 먼저 배치
					inputKeyStr += nKey2;
				}
				else {
					inputKeyStr = nKey2;
					inputKeyStr += nKey;
				}
				return inputKeyStr;
			}
		}
	}
	return "";
}
int main(void) {
	int nKey;
	SoundSystem(); // FMOD 사용 준비
	ScreenInit();
	KeyIndexInit();
	init(); // 초기화
	Play(0);
	while (1) {
		if (_kbhit()) {
			nKey = _getch();
			if (nKey == '\r') {
				if (Stage == READY) {
					pChannel[0]->stop();
					Play(1);
				}
				else if (Stage == PAUSE) {
					PauseEnd = clock();
					PauseTime += PauseEnd - PauseStart;
					pChannel[0]->setPaused(false);
				}
		
				Stage = RUNNING; // 엔터 입력 시 running시작 음악 호출
			}
			if (nKey == 'p') {
				if (Stage == RUNNING) {
					PauseStart = clock();
					pChannel[0]->setPaused(true);
					Stage = PAUSE;
				}
			}
			if (nKey == 'a' || nKey == 's' || nKey == 'd' || nKey == 'j' || nKey == 'k' || nKey == 'l') {
				if (Stage == PAUSE) { continue; }
				
				string inputKeyStr; // CheckKey()의 인자로 줄 string 변수 선언 
				inputKeyStr = nKey; // nKey를 string 변수에 대입 
				if (isTwoKey(Note[n]) || (n>0 && isTwoKey(Note[n - 1])) || isTwoKey(Note[n + 1])) { //현재 노트가 두 개라면
					inputKeyStr=secondkbhit(nKey, inputKeyStr); // 첫 번째 키와 비교를 위한 int nKey와 string 반환을 위한 string inputKeyStr을 변수로 줌 
				}
				CheckKey(inputKeyStr);
			}
		}

		Update();  // 데이터 갱신
		Render();  // 화면출력

		
	}
	Release(); // 해제
	ScreenRelease();
	return 0;
}




// 악보
void NoteCheck(void) {
	for (int i = 0; i < 30; i++) {
		Note[i] = " ";
	}

	for (int i = 0; i < 121; i += 40)
	{
		Note[30 + i + Control.nMagic] = nKeyL;
		Note[40 + i + Control.nMagic] = nKeyD;
		Note[50 + i + Control.nMagic] = nKeyL;
		Note[60 + i + Control.nMagic] = nKeyS;
	}
	///////////// L > D > L > S ///// 10간격으로 4번 반복 

	Note[195 + Control.nMagic] = nKeyAJ;  // 14초 경과

	Note[208 + Control.nMagic] = nKeySK;
	Note[213 + Control.nMagic] = nKeyDL;
	Note[218 + Control.nMagic] = nKeySK;
	Note[223 + Control.nMagic] = nKeyAJ;

	Note[228 + Control.nMagic] = nKeySK;
	Note[233 + Control.nMagic] = nKeyDL;

	Note[250 + Control.nMagic] = nKeySK;
	Note[252 + Control.nMagic] = nKeyAJ;

	Note[266 + Control.nMagic] = nKeyDL;
	Note[279 + Control.nMagic] = nKeySK;
	Note[274 + Control.nMagic] = nKeyAJ;
	Note[278 + Control.nMagic] = nKeyDL;    // 22초

	Note[295 + Control.nMagic] = nKeySK;

	Note[302 + Control.nMagic] = nKeyDL;
	Note[306 + Control.nMagic] = nKeySK;
	Note[310 + Control.nMagic] = nKeyDL;
	Note[314 + Control.nMagic] = nKeySK;
	Note[318 + Control.nMagic] = nKeyAJ;

	//연속구간

	for (int i = 0; i < 7; i++)
	{
		Note[320 + i + Control.nMagic] = nKeyAJ;
	}

	Note[334 + Control.nMagic] = nKeyL;
	Note[336 + Control.nMagic] = nKeyK;
	Note[338 + Control.nMagic] = nKeyJ;

	//Note[351 + Control.nMagic] = nKeyA; // 26초 경과

	for (int i = 0; i < 25 ; i += 12)
	{
		Note[353 + i + Control.nMagic] = nKeyS; 
		Note[358 + i + Control.nMagic] = nKeyDL;
	}
	Note[389 + Control.nMagic] = nKeyS;

	for (int i = 0; i < 20; i += 9)
	{
		Note[400 + i + Control.nMagic] = nKeyS;
		Note[404 + i + Control.nMagic] = nKeyDL;
	}
	for (int i = 0; i <= 10; i++)
	{
		Note[440 + i + Control.nMagic] = nKeyA;
	}

	Note[461 + Control.nMagic] = nKeyJ;
	Note[462 + Control.nMagic] = nKeyK;
	Note[463 + Control.nMagic] = nKeyL;
	Note[470 + Control.nMagic] = nKeyA;

	Note[477 + Control.nMagic] = nKeyJ;
	Note[480 + Control.nMagic] = nKeyK;
	Note[483 + Control.nMagic] = nKeyL;
	Note[490 + Control.nMagic] = nKeyA;

	Note[496 + Control.nMagic] = nKeyDL; //원래 D였음, AD면좋을텐뎅

	for (int i = 0; i < 7; i += 6)
	{
		Note[501 + i + Control.nMagic] = nKeyJ;
		Note[504 + i + Control.nMagic] = nKeyK;
	}

	Note[514 + Control.nMagic] = nKeyA;
	Note[517 + Control.nMagic] = nKeyD;

	Note[523 + Control.nMagic] = nKeyJ;
	Note[527 + Control.nMagic] = nKeyK;
	Note[531 + Control.nMagic] = nKeyL;

	Note[536 + Control.nMagic] = nKeyA;
	Note[540 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 7; i += 6)
	{
		Note[543 + i + Control.nMagic] = nKeyJ;
		Note[545 + i + Control.nMagic] = nKeyK;
		Note[547 + i + Control.nMagic] = nKeyL;
	}

	Note[558 + Control.nMagic] = nKeyA;
	Note[563 + Control.nMagic] = nKeyD;

	Note[566 + Control.nMagic] = nKeyA;
	Note[569 + Control.nMagic] = nKeyK;
	Note[572 + Control.nMagic] = nKeyL;

	Note[578 + Control.nMagic] = nKeyA;
	//Note[582 + Control.nMagic] = nKeyD;

	for (int i = 0; i <= 10; i += 4)
	{
		Note[586 + i + Control.nMagic] = nKeyJ;
		Note[588 + i + Control.nMagic] = nKeyK;
	}

	Note[606 + Control.nMagic] = nKeyL;
	Note[608 + Control.nMagic] = nKeyK;
	Note[610 + Control.nMagic] = nKeyJ;
	Note[612 + Control.nMagic] = nKeyD;
	Note[614 + Control.nMagic] = nKeyS;
	Note[616 + Control.nMagic] = nKeyA;

	Note[631 + Control.nMagic] = nKeyJ;
	Note[632 + Control.nMagic] = nKeyK;

	for (int i = 0; i<6; i++)
	{
		Note[633 + i + Control.nMagic] = nKeyL;
	}
	////////////

	Note[645 + Control.nMagic] = nKeyJ;
	Note[648 + Control.nMagic] = nKeyK;
	Note[651 + Control.nMagic] = nKeyL;

	Note[659 + Control.nMagic] = nKeyA;
	Note[664 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 7; i += 6)
	{
		Note[669 + i + Control.nMagic] = nKeyJ;
		Note[672 + i + Control.nMagic] = nKeyK;
	}

	Note[682 + Control.nMagic] = nKeyA;
	Note[685 + Control.nMagic] = nKeyD;

	Note[691 + Control.nMagic] = nKeyJ;
	Note[695 + Control.nMagic] = nKeyK;
	Note[699 + Control.nMagic] = nKeyL;

	Note[704 + Control.nMagic] = nKeyA;
	Note[708 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 7; i += 6)
	{
		Note[711 + i + Control.nMagic] = nKeyJ;
		Note[713 + i + Control.nMagic] = nKeyK;
		Note[715 + i + Control.nMagic] = nKeyL;
	}

	Note[726 + Control.nMagic] = nKeyJ;
	Note[729 + Control.nMagic] = nKeyK;
	Note[732 + Control.nMagic] = nKeyL;

	Note[737 + Control.nMagic] = nKeyA;
	Note[740 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 9; i += 4)
	{
		Note[745 + Control.nMagic] = nKeyJ;
		Note[747 + Control.nMagic] = nKeyK;
	}

	for (int i = 0; i < 11; i += 10)
	{
		Note[760 + i + Control.nMagic] = nKeyS;
		Note[765 + i + Control.nMagic] = nKeyK;
	}
	
	for (int i = 0; i < 6; i++)
	{
		Note[778 + i + Control.nMagic] = nKeySK;
	}

}

//키와 노트 string를 KeyNote구조체에 초기화 시켜주는 함수
void KeyIndexInit() {
	//index는 입력받은 키의 종류 (note.h에 상수로 선언)
	KeyIndex[none].inputKey = "none";
	KeyIndex[none].nKey = "                                      ";
	KeyIndex[a].inputKey = "a";
	KeyIndex[a].nKey = "■■■";
	KeyIndex[s].inputKey = "s";
	KeyIndex[s].nKey = "      ■■■";
	KeyIndex[d].inputKey = "d";
	KeyIndex[d].nKey = "            ■■■";
	KeyIndex[j].inputKey = "j";
	KeyIndex[j].nKey = "                    ■■■";
	KeyIndex[k].inputKey = "k";
	KeyIndex[k].nKey = "                          ■■■";
	KeyIndex[l].inputKey = "l";
	KeyIndex[l].nKey = "                                ■■■";
	KeyIndex[aj].inputKey = "aj";
	KeyIndex[aj].nKey = "■■■              ■■■";
    KeyIndex[ks].inputKey = "ks";
	KeyIndex[ks].nKey = "      ■■■              ■■■";

	KeyIndex[dl].inputKey = "dl";
	KeyIndex[dl].nKey = "            ■■■              ■■■";
}

// 키의 문자열을 반환해주는 함수
string GetKeyType(string nKey) {
	string inputKeyStr="";
	for (int i = 0; i < NumOfKey; i++) {
		if (nKey == KeyIndex[i].inputKey) {
			inputKeyStr = KeyIndex[i].nKey;
			break;
		}
	}
	return inputKeyStr;
}

// 현재 노트가 두 개의 노트인지 확인해주는 함수
int isTwoKey(string note) {
	for (int i = 0; i < NumOfKey; i++) {
		if (note == KeyIndex[i].nKey) { //note가 KeyIndex 구조체의 nKey string과 같다면
			if (i >= aj) { //상수 aj보다 크다면 두 개의 노트를 가지고 있으므로 1 리턴
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	return 0;
}

// 충돌처리
// main에서 해당 키 입력시 호출되는 함수
void CheckKey(string inputKey) {
	checkstring = inputKey;
	string inputKeyStr; // 입력한 키의 종류
	inputKeyStr = GetKeyType(inputKey);
	if (Note[n] == inputKeyStr) { // Perfect판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if ((n > 0 && (Note[n - 1] == inputKeyStr)) || (Note[n + 1] == inputKeyStr)) { // Great 판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}
