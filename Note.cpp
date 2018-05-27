#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include<memory.h>
#include<conio.h>
#include<string>
#include"fmod.hpp"
#include"Screen.h"
#include "Note.h"

using namespace std;
using namespace FMOD;



//사운드 함수
void SoundSystem() {
	System_Create(&pSystem);

	pSystem->init(4, FMOD_INIT_NORMAL, NULL);

	pSystem->createSound("opening.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[0]); // 오프닝음악
	pSystem->createSound("Festival_of_Ghost.wav", FMOD_DEFAULT, NULL, &pSound[1]); // 게임음악
}
void Play(int Sound_num) {

	pSystem->playSound(FMOD_CHANNEL_FREE, pSound[Sound_num], 0, pChannel);

}

// 맵 함수
// 스테이지 기본 틀
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "□□□□□□□□□□□□□□□□□□□□□");
	for (int i = 1; i < 29; i++) {
		ScreenPrint(0, i, "□\t\t\t\t\t□");
	}
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
// 게임 실행 전 준비화면
void ReadyMap() {
	ScreenPrint(15, 10, "유령의 축제");
	ScreenPrint(2, 26, "□□□■■■□□□  ■■■□□□■■■");
	ScreenPrint(11, 20, "Press c to Syncmap");
	ScreenPrint(2, 27, "  A     S     D       J     K      L");
	// 게임 조작 키 설명 
}
// Render함수에서 깜빡이면서 출력
void ReadyMap1() {
	SetColor(10);
	ScreenPrint(10, 15, "Press Enter to Start");

	SetColor(15);
}
// 결과화면 출력
void ResultMap()
{
	SetColor(9); ScreenPrint(15, 10, "GAME END !"); SetColor(15);
	UserScore = "[User] "; // 사용자 점수를 나타냄

	UserScore += to_string(nScore);

	if (nScore >= bestScore) {
		BestScore = "[Best] ";
		bestScore = nScore;
		BestScore += to_string(bestScore);
	}

	SetColor(14);
	ScreenPrint(13, 12, BestScore);
	SetColor(15);
	ScreenPrint(13, 14, UserScore);
	SetColor(10);
	ScreenPrint(10, 18, "Press Enter to Restart");
	ScreenPrint(13, 20, "Press q to Exit");
	SetColor(15);
}
// 싱크화면 출력
void SyncMap()
{
	ScreenPrint(4, 15, "Press ← → to tune the Sync! : "+to_string(Syncnum));

	ScreenPrint(10, 20, "Press Enter to Start ");
}

// 초기화 함수
//초기 시간과 노트들 그리고 스코어 초기화
void init() {
	curNoteIndex = 0; 
	nScore = 0; //
	nCombo = 0;
	Control.MovTime = 52;
	Control.OldTime = 0;
	Control.nMagic = 1;
	Stage = READY;

	RunningTime = 0;
	NoteInit();
	SyncTime = 0;
	PauseTime = 0;
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
// 악보
void NoteInit(void) {
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

	for (int i = 0; i < 7; i += 2)
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
	for (int i = 0; i <= 10; i += 2)
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

	for (int i = 0; i <= 6; i += 2)
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

	for (int i = 0; i < 9; i += 2)
	{
		Note[777 + i + Control.nMagic] = nKeySK;
	}

}
// 기본 기능 함수
void Update() {
	clock_t Curtime = clock();
	switch (Stage) {
	case READY:
		Oldtime = Curtime;
		break;
	case RUNNING:
		// 게임 시작 후 시간 측정변수
		RunningTime = Curtime - Oldtime - PauseTime - SyncTime;

		break;
	case PAUSE:
		break;
	}
}
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
				curNoteIndex++;//노트가 저장된 배열의 인덱스를 증가
			}
			ShowNote();
		}
		if (RunningTime > 52000)
		{
			Stage = RESULT;
		}
		break;
	case RESULT:
		ResultMap();
		break;
	}






	ScreenFlipping();
}
void Release() {

}

// 키 함수
// 두 번째 키 입력을 받는 함수
string secondkbhit(int inputKey, string inputKeyStr) {
	int inputKey2; // 두 번째로 입력된 키를 받기 위한 변수
	for (int i = 0; i < 1000; i++) {
		if (_kbhit()) {
			inputKey2 = _getch();
			if (inputKey2 == 'a' || inputKey2 == 's' || inputKey2 == 'd' || inputKey2 == 'j' || inputKey2 == 'k' || inputKey2 == 'l') {
				if (inputKey == inputKey2) { //이전 키가 길게 눌렸을 경우 인식하는 것을 방지
					continue;
				}
				else if (inputKey < inputKey2) { //알파벳 순서가 빠른 것을 inputKeyStr에 먼저 배치
					inputKeyStr += inputKey2;
				}
				else {
					inputKeyStr = inputKey2;
					inputKeyStr += inputKey;
				}
				break;
			}
		}
	}
	return inputKeyStr;
}
// 키에 해당하는 노트 문자열을 반환해주는 함수
string GetKeyType(string inputKeyStr) {
	string nKeyStr = "";
	for (int i = 0; i < NumOfKey; i++) {
		if (inputKeyStr == KeyIndex[i].inputKey) {
			nKeyStr = KeyIndex[i].nKey;
			break;
		}
	}
	return nKeyStr;
}
// 현재 노트가 두 개의 노트인지 확인해주는 함수
int isTwoKey(string nKeyStr) {
	for (int i = 0; i < NumOfKey; i++) {
		if (nKeyStr == KeyIndex[i].nKey) { //note가 KeyIndex 구조체의 nKey string과 같다면
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
void CheckKey(string inputKeyStr) {
	string nKeyStr; // 입력한 키의 종류
	nKeyStr = GetKeyType(inputKeyStr);
	if (Note[curNoteIndex] == nKeyStr) { // Perfect판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 500;
		nCombo++;
		if (nCombo >= 10) {
			nScore += nCombo * 50;
		}
		Note[curNoteIndex] = HitNote(nKeyStr);
		sprintf(strScore, "%s", "★Perfect★");
	}
	else if ((curNoteIndex > 0 && (Note[curNoteIndex - 1] == nKeyStr)) || (Note[curNoteIndex + 1] == nKeyStr)) { // Great 판별 구간의 Note와 입력한 KeyType가 일치하는 경우
		nScore += 300;
		nCombo++;
		if (nCombo >= 10) {
			nScore += nCombo * 30;
		}
		Note[curNoteIndex + 1] = HitNote(nKeyStr);
		Note[curNoteIndex - 1] = HitNote(nKeyStr);
		sprintf(strScore, "%s", "★Great★");
	}
	else {
		nCombo = 0;
	}
}

// 노트 함수
// 2차원 배열을 아래로 떨어지게끔 해주는 함수
void ShowNote() {
	for (int i = 0; i < 27; i++) {
		if (28 - i >= 27) { // 히트 라인 밑으로 내려간 블록 전부다 노란색으로 변경.
			SetColor(14);
		}
		else SetColor(15);
		ScreenPrint(2, 28 - i, Note[curNoteIndex + i]);
	}
}
// 히트를 성공한 노트를 히트된 노트 string으로 반환
string HitNote(string inputKey) {
	for (int i = 0; i < inputKey.length(); i++) {
		if (inputKey[i] != ' ') {
			inputKey[i] = 'oo';
		}
	}
	return inputKey;
}

// 싱크 함수
void ControlSync(int inputKey) {
	if (inputKey == LEFT) { // 싱크 줄이기
		if (Syncnum <= -30) {
			Syncnum = Syncnum;
		}
		else {
			Syncnum--;
		}
	}
	else  { // 싱크 높이기
		if (Syncnum >= 30) {
			Syncnum = Syncnum;
		}
		else {
			Syncnum++;
		}
	}
	Control.nMagic = Syncnum;
}