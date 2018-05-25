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

string star = "";
int star_x = 2;
//
clock_t RunningTime;
clock_t PauseStart;
clock_t PauseEnd;
clock_t PauseTime = 0;

 //노트에 해당하는 변수 선언



// 19개 / 3 / 3 / 3 / 1 / 3 / 3 / 3

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
	READY, RUNNING,PAUSE, RESULT , SYNC
}STAGE;
STAGE Stage;



// 스테이지 기본 틀
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "□□□□□□□□□□□□□□□□□□□□□");
	for (int i = 1; i < 29; i++) {
		ScreenPrint(0, i, "□\t\t\t\t\t□");
	}
	ScreenPrint(star_x, 28, star.c_str());
	ScreenPrint(0, 29, "□□□□□□□□□□□□□□□□□□□□□");
	ScreenPrint(2, 26, "______________________________________");
}

void Hitmap(int nkey)
{
	if (nkey == 'a')
	{
		star = "☆";
		star_x = 4;
	}
	else if (nkey == 's')
	{
		star = "☆";
		star_x = 10;
	}
	else if (nkey == 'd')
	{
		star = "☆";
		star_x = 16;
	}
	else if (nkey == 'j')
	{
		star = "☆";
		star_x = 24;
	}
	else if (nkey == 'k')
	{
		star = "☆";
		star_x = 30;
	}
	else if (nkey == 'l')
	{
		star = "☆";
		star_x = 36;
	}
}


// 우측 점수 출력틀
void ScoreMap() {
	// 경과시간
	char nTime[20];//경과 시간을 나타낸다
	sprintf(nTime, "시간 : %d.%d초", RunningTime / 1000, RunningTime % 1000);
	ScreenPrint(44, 2, nTime);
	// 점수 목록
	if (strcmp(strScore, "★Perfect★") == 0) {
		SetColor(14);
	}
	else if (strcmp(strScore, "★Great★") == 0) {
		SetColor(9);
	}
	ScreenPrint(44, 10, strScore);//Great,Perfect판별
	SetColor(15);
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
	ScreenPrint(15, 20, "싱크 조정");
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



string Note[ALLNOTE];
void NoteCheck(void);



// 2차원 배열을 아래로 떨어지게끔 해주는 함수
void ShowNote(int n) {
	for (int i = 0; i < 27; i++) {
		ScreenPrint(2, 27-i, Note[n+i]);
	}
}



// 입력 키 판별해주는 함수
void CheckKey(int nKey);




void init() {
	Control.MovTime = 52;
	Control.OldTime = 0;
	Control.nMagic = 1;
	Stage = READY;
	
	/*for (int i = 0; i < ALLNOTE; i++) {
		Note[i] = " ";
	}*/ // 노트 초기화 할 필요 있나? noteCheck 에서 노트들 모두 초기화해주기때문에
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
	Control.nMagic = 1;
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
void Render(int nkey) {
	clock_t Curtime = clock(); // 지금까지 흐른 시간
	ScreenClear();
	//출력코드
	Map();
	Hitmap(nkey);
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
int main(void) {
	int nKey = 0;
	SoundSystem(); // FMOD 사용 준비
	ScreenInit();
	init(); // stage를 ready 상태로 만들고 노트들 초기화
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
			/*if (nKey == 'c')
			{
				if (Stage == RUNNING)
				{
				}
				else if (Stage == READY)
				{
				}
			}*/

			if (nKey == 'a' || nKey == 's' || nKey == 'd' || nKey == 'j' || nKey == 'k' || nKey == 'l') {
				if (Stage == PAUSE) continue;
				CheckKey(nKey);
			}
			
		}

		Update();  // 데이터 갱신
		Render(nKey);  // 화면출력

		
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

	Note[206 + Control.nMagic] = nKeySK;
	Note[211 + Control.nMagic] = nKeyDL;
	Note[217 + Control.nMagic] = nKeySK;
	Note[221 + Control.nMagic] = nKeyAJ;

	Note[226 + Control.nMagic] = nKeySK;
	Note[231 + Control.nMagic] = nKeyDL;

	Note[238 + Control.nMagic] = nKeySK;
	Note[250 + Control.nMagic] = nKeyAJ;

	Note[262 + Control.nMagic] = nKeyDL;
	Note[275 + Control.nMagic] = nKeySK;
	Note[270 + Control.nMagic] = nKeyAJ;
	Note[274 + Control.nMagic] = nKeyDL;    // 22초

	Note[290 + Control.nMagic] = nKeySK;

	Note[298 + Control.nMagic] = nKeyDL;
	Note[302 + Control.nMagic] = nKeySK;
	Note[306 + Control.nMagic] = nKeyDL;
	Note[310 + Control.nMagic] = nKeySK;
	Note[314 + Control.nMagic] = nKeyAJ;

	//연속구간

	for (int i = 0; i < 7; i++)
	{
		Note[318 + i + Control.nMagic] = nKeyAJ;
	}

	Note[330 + Control.nMagic] = nKeyL;
	Note[332 + Control.nMagic] = nKeyK;
	Note[334 + Control.nMagic] = nKeyJ;

	//Note[351 + Control.nMagic] = nKeyA; // 26초 경과

	for (int i = 0; i < 25 ; i += 12)
	{
		Note[349 + i + Control.nMagic] = nKeyS; 
		Note[354 + i + Control.nMagic] = nKeyDL;
	}
	Note[384 + Control.nMagic] = nKeyS;

	for (int i = 0; i < 20; i += 9)
	{
		Note[396 + i + Control.nMagic] = nKeyS;
		Note[400 + i + Control.nMagic] = nKeyDL;
	}
	for (int i = 0; i <= 10; i++)
	{
		Note[436 + i + Control.nMagic] = nKeyA;
	}

	Note[456 + Control.nMagic] = nKeyJ;
	Note[457 + Control.nMagic] = nKeyK;
	Note[458 + Control.nMagic] = nKeyL;
	Note[465 + Control.nMagic] = nKeyA;

	Note[473 + Control.nMagic] = nKeyJ;
	Note[476 + Control.nMagic] = nKeyK;
	Note[479 + Control.nMagic] = nKeyL;
	Note[486 + Control.nMagic] = nKeyA;

	Note[492 + Control.nMagic] = nKeyDL; 

	for (int i = 0; i < 7; i += 6)
	{
		Note[497 + i + Control.nMagic] = nKeyJ;
		Note[500 + i + Control.nMagic] = nKeyK;
	}

	Note[510 + Control.nMagic] = nKeyA;
	Note[513 + Control.nMagic] = nKeyD;

	Note[519 + Control.nMagic] = nKeyJ;
	Note[523 + Control.nMagic] = nKeyK;
	Note[527 + Control.nMagic] = nKeyL;

	Note[532 + Control.nMagic] = nKeyA;
	Note[536 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 7; i += 6)
	{
		Note[539 + i + Control.nMagic] = nKeyJ;
		Note[541 + i + Control.nMagic] = nKeyK;
		Note[545 + i + Control.nMagic] = nKeyL;
	}

	Note[553 + Control.nMagic] = nKeyA;
	Note[558 + Control.nMagic] = nKeyD;

	Note[562 + Control.nMagic] = nKeyA;
	Note[565 + Control.nMagic] = nKeyK;
	Note[568 + Control.nMagic] = nKeyL;

	Note[574 + Control.nMagic] = nKeyA;

	for (int i = 0; i <= 10; i += 4)
	{
		Note[582 + i + Control.nMagic] = nKeyJ;
		Note[584 + i + Control.nMagic] = nKeyK;
	}

	Note[602 + Control.nMagic] = nKeyL;
	Note[604 + Control.nMagic] = nKeyK;
	Note[606 + Control.nMagic] = nKeyJ;
	Note[608 + Control.nMagic] = nKeyD;
	Note[610 + Control.nMagic] = nKeyS;
	Note[612 + Control.nMagic] = nKeyA;

	Note[627 + Control.nMagic] = nKeyJ;
	Note[628 + Control.nMagic] = nKeyK;

	for (int i = 0; i<6; i++)
	{
		Note[629 + i + Control.nMagic] = nKeyL;
	}
	////////////

	Note[641 + Control.nMagic] = nKeyJ;
	Note[644 + Control.nMagic] = nKeyK;
	Note[647 + Control.nMagic] = nKeyL;

	Note[655 + Control.nMagic] = nKeyA;
	Note[660 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 7; i += 6)
	{
		Note[665 + i + Control.nMagic] = nKeyJ;
		Note[668 + i + Control.nMagic] = nKeyK;
	}

	Note[678 + Control.nMagic] = nKeyA;
	Note[681 + Control.nMagic] = nKeyD;

	Note[687 + Control.nMagic] = nKeyJ;
	Note[691 + Control.nMagic] = nKeyK;
	Note[695 + Control.nMagic] = nKeyL;

	Note[700 + Control.nMagic] = nKeyA;
	Note[704 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 7; i += 6)
	{
		Note[707 + i + Control.nMagic] = nKeyJ;
		Note[709 + i + Control.nMagic] = nKeyK;
		Note[711 + i + Control.nMagic] = nKeyL;
	}

	Note[722 + Control.nMagic] = nKeyJ;
	Note[725 + Control.nMagic] = nKeyK;
	Note[728 + Control.nMagic] = nKeyL;

	Note[733 + Control.nMagic] = nKeyA;
	Note[737 + Control.nMagic] = nKeyD;

	for (int i = 0; i < 9; i += 4)
	{
		Note[741 + i+ Control.nMagic] = nKeyJ;
		Note[743 + i+Control.nMagic] = nKeyK;
	}

	for (int i = 0; i < 11; i += 10)
	{
		Note[757 + i + Control.nMagic] = nKeyS;
		Note[762 + i + Control.nMagic] = nKeyK;
	}
	
	for (int i = 0; i < 6; i++)
	{
		Note[777 + i + Control.nMagic] = nKeySK;
	}

}

// 키의 문자열 포인터를 반환해주는 함수
string GetKeyType(int nKey) {
	string KeyType;
	switch (nKey) {
	case 'a':
		KeyType = nKeyA;
		break;
	case 's':
		KeyType = nKeyS;
		break;
	case 'd':
		KeyType = nKeyD;
		break;
	case 'j':
		KeyType = nKeyJ;
		break;
	case 'k':
		KeyType = nKeyK;
		break;
	case 'l':
		KeyType = nKeyL;
		break;
	default:
		KeyType = nKeyNone;
		break;
	}
	return KeyType;
}

// 충돌처리
// main에서 해당 키 입력시 호출되는 함수

void CheckKey(int nKey) {
	string KeyType; // 입력한 키의 종류
	KeyType = GetKeyType(nKey);
	if (Note[n] == KeyType) { // Perfect판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "★Perfect★");
		
		
	}
	else if ((n > 0 && (Note[n - 1] == KeyType)) || (Note[n + 1] == KeyType)) { // Great 판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "★Great★");
		
	}
	else {
		nCombo = 0;
	}
}
