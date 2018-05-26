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
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ALLNOTE 1300
//

System* pSystem;
Sound* pSound[2];
Channel* pChannel[1];

void SoundSystem() {
	System_Create(&pSystem);

	pSystem->init(4, FMOD_INIT_NORMAL, NULL);

	pSystem->createSound("opening.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[0]); // 오프닝음악
	pSystem->createSound("Festival_of_Ghost.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[1]); // 게임음악
}

void Play(int Sound_num) {

	pSystem->playSound(FMOD_CHANNEL_FREE, pSound[Sound_num], 0, pChannel);

}

//
int n = 0;

int nScore = 0;
char strScore[20] = "  ";
int nCombo = 0;
int Syncnum = 1;
string star = "";
int star_x = 2;
//
clock_t RunningTime;
clock_t PauseStart;
clock_t PauseEnd;
clock_t PauseTime = 0;
clock_t SyncStart;
clock_t SyncEnd;
clock_t SyncTime = 0;
string Sync1 = "화살표로 싱크를 조정하세요! : ";
string Sync2 = "화살표로 싱크를 조정하세요! :";
string Sync3 = "";


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
	READY, RUNNING, PAUSE, RESULT, SYNC
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

	ScreenPrint(9, 7, "┌---------------------┐");
	for (int i = 8; i < 15; i++) {
		ScreenPrint(9, i, "│\t\t       │");
	}
	SetColor(9); ScreenPrint(15, 10, "GAME END !"); SetColor(15);
	char UserScore[20];//사용자 점수를 나타냄
	sprintf(UserScore, "Score : %d 점", nScore);
	ScreenPrint(14, 12, UserScore);
	ScreenPrint(9, 15, "└---------------------┘");

}
void SyncMap()
{
	ScreenPrint(15, 15, Sync2);
}



string Note[ALLNOTE];
void KeyIndexInit();
int isTwoKey(string note);

void CheckKey(string inputKey);
void NoteCheck(void);



// 2차원 배열을 아래로 떨어지게끔 해주는 함수
void ShowNote(int n) {
	for (int i = 0; i < 27; i++) {
		if (28 - i >= 27) { // 히트 라인 밑으로 내려간 블록 전부다 노란색으로 변경.
			SetColor(14);
		}
		else SetColor(15);
		ScreenPrint(2, 28 - i, Note[n + i]);
	}
}


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
	//Control.nMagic = 1;
	switch (Stage) {
	case READY:
		Oldtime = Curtime;
		break;
	case RUNNING:
		// 게임 시작 후 시간 측정변수
		RunningTime = Curtime - Oldtime - PauseTime - SyncTime ;

		break;
	case PAUSE:
		break;
		//case RESULT:


		//break;
	}
	//NoteCheck();
}





clock_t Oldtime1 = 0;
void Render(int nkey) {
	clock_t Curtime = clock(); // 지금까지 흐른 시간
	ScreenClear();
	//출력코드
	
	Map();
	ScoreMap();
	if (Stage == SYNC)
	{
		SyncMap();
	}
	switch (Stage) {
	case READY://대기상태
		Oldtime1 = Curtime;
		ReadyMap();
		if (Curtime % 1000 > 500) {
			ReadyMap1();
		}//0.5초 단위로 화면을 출력
		break;
	case PAUSE:
		return;
	case RUNNING:
		if (RunningTime > 3100) //3초 이후부터
		{
			if (Curtime - Control.OldTime > Control.MovTime)
			{
				Control.OldTime = Curtime;
				n++;//노트가 저장된 배열의 인덱스를 증가
			}
			ShowNote(n);
		}
		if (RunningTime > 52000)
		{
			Stage = RESULT;
		}
		break;
	case RESULT:
		if (RunningTime > 52000)
		{
			ResultMap();
		}

		break;
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
	int nKey = 0;
	SoundSystem(); // FMOD 사용 준비
	ScreenInit();
	KeyIndexInit();
	init(); // stage를 ready 상태로 만들고 노트들 초기화
	Play(0); // pSound[0] (=opening.wav)를 실행
	
	while (1) {
		if (_kbhit()) {
			nKey = _getch();
			if (nKey == '\r') {
				if (Stage == READY) {
					pChannel[0]->stop();
					Play(1); // pSound[0] (=Festival_of_Ghost.wav)를 실행
				}
				else if (Stage == PAUSE) {
					PauseEnd = clock();
					PauseTime += PauseEnd - PauseStart;
					pChannel[0]->setPaused(false); // 현재 pChannel[0]에 있는 노래의 일시 정지를 해제한다.
				}
				else if (Stage == SYNC)
				{
					NoteCheck();
					pChannel[0]->stop();
					Play(1);
					SyncEnd = clock();
					SyncTime += SyncEnd - SyncStart;
				}

				Stage = RUNNING; // 엔터 입력 시 running시작 음악 호출
			}

			if (nKey == 'p') {
				if (Stage == RUNNING) {
					PauseStart = clock();
					pChannel[0]->setPaused(true); // 현재 pChannel[0]에 재생중인 노래를 일시 정지한다.
					Stage = PAUSE;
				}
			}
			if (nKey == 'c')
			{
				SyncStart = clock();
				Stage = SYNC;
				SyncMap();
			}

			if (nKey == 'a' || nKey == 's' || nKey == 'd' || nKey == 'j' || nKey == 'k' || nKey == 'l') {
				if (Stage == PAUSE) { continue; }

				string inputKeyStr; // CheckKey()의 인자로 줄 string 변수 선언 
				inputKeyStr = nKey; // nKey를 string 변수에 대입 
				if (isTwoKey(Note[n]) || (n>0 && isTwoKey(Note[n - 1])) || isTwoKey(Note[n + 1])) { //현재 노트가 두 개라면
					inputKeyStr = secondkbhit(nKey, inputKeyStr); // 첫 번째 키와 비교를 위한 int nKey와 string 반환을 위한 string inputKeyStr을 변수로 줌 
				}
				CheckKey(inputKeyStr);
			}
			if (Stage == SYNC)
			{

				if (nKey == LEFT)// 싱크 줄이기
				{
					Sync2 = Sync1;
					Syncnum--;

				}
				else if (nKey == RIGHT) // 싱크 높이기
				{
					Sync2 = Sync1;
					Syncnum++;
				}
				else
				{
					continue;
				}

				Sync3 = to_string(Syncnum);
				Sync2 += Sync3;
				Control.nMagic = Syncnum;


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
	for (int i = 0; i < ALLNOTE; i++) {
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

	for (int i = 0; i < 25; i += 12)
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
		Note[741 + i + Control.nMagic] = nKeyJ;
		Note[743 + i + Control.nMagic] = nKeyK;
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
	string inputKeyStr = "";
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

string HitNote(string inputKey) {
	for (int i = 0; i < inputKey.length(); i++) {
		if (inputKey[i] != ' ') {
			inputKey[i] = 'oo';
		}
	}
	return inputKey;
}
// 충돌처리
// main에서 해당 키 입력시 호출되는 함수
void CheckKey(string inputKey) {
	string inputKeyStr; // 입력한 키의 종류
	inputKeyStr = GetKeyType(inputKey);
	if (Note[n] == inputKeyStr) { // Perfect판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 500;
		nCombo++;
		Note[n] = HitNote(inputKeyStr);
		sprintf(strScore, "%s", "★Perfect★");


	}
	else if ((n > 0 && (Note[n - 1] == inputKeyStr)) || (Note[n + 1] == inputKeyStr)) { // Great 판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 300;
		nCombo++;
		Note[n + 1] = HitNote(inputKeyStr);
		Note[n - 1] = HitNote(inputKeyStr);
		sprintf(strScore, "%s", "★Great★");

	}
	else {
		nCombo = 0;
	}
}