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

	pSystem->createSound("opening.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[0]); // �������
	pSystem->createSound("Festival_of_Ghost.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[1]); // �������
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

 //��Ʈ�� �ش��ϴ� ���� ����



// 19�� / 3 / 3 / 3 / 1 / 3 / 3 / 3

// ��Ʈ �Ǻ� ��
typedef struct _NOTECOUNT {
	int nXofA;   //(2,29)
	int nXofS;
	int nXofD;
	int nXofJ;
	int nXofK;
	int nXofL;
}NOTECOUNT;
NOTECOUNT Count;

// �������� ����
typedef enum _STAGE {
	READY, RUNNING,PAUSE, RESULT , SYNC
}STAGE;
STAGE Stage;



// �������� �⺻ Ʋ
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "����������������������");
	for (int i = 1; i < 29; i++) {
		ScreenPrint(0, i, "��\t\t\t\t\t��");
	}
	ScreenPrint(star_x, 28, star.c_str());
	ScreenPrint(0, 29, "����������������������");
	ScreenPrint(2, 26, "______________________________________");
}

void Hitmap(int nkey)
{
	if (nkey == 'a')
	{
		star = "��";
		star_x = 4;
	}
	else if (nkey == 's')
	{
		star = "��";
		star_x = 10;
	}
	else if (nkey == 'd')
	{
		star = "��";
		star_x = 16;
	}
	else if (nkey == 'j')
	{
		star = "��";
		star_x = 24;
	}
	else if (nkey == 'k')
	{
		star = "��";
		star_x = 30;
	}
	else if (nkey == 'l')
	{
		star = "��";
		star_x = 36;
	}
}


// ���� ���� ���Ʋ
void ScoreMap() {
	// ����ð�
	char nTime[20];//��� �ð��� ��Ÿ����
	sprintf(nTime, "�ð� : %d.%d��", RunningTime / 1000, RunningTime % 1000);
	ScreenPrint(44, 2, nTime);
	// ���� ���
	if (strcmp(strScore, "��Perfect��") == 0) {
		SetColor(14);
	}
	else if (strcmp(strScore, "��Great��") == 0) {
		SetColor(9);
	}
	ScreenPrint(44, 10, strScore);//Great,Perfect�Ǻ�
	SetColor(15);
	ScreenPrint(44, 22, "Great : 300��");
	ScreenPrint(44, 23, "Perfect : 500��");
	SetColor(12);
	ScreenPrint(44, 6, "Press \'p\' to Pause");
	SetColor(15);
	// ����
	char UserScore[20];//����� ������ ��Ÿ��
	sprintf(UserScore, "���� : %d ��", nScore);
	ScreenPrint(44, 4, UserScore);
	ScreenPrint(44, 27, "<<< ��Ʈ ����(G)");
	ScreenPrint(44, 28, "<<< ��Ʈ ����(P)");
	ScreenPrint(44, 29, "<<< ��Ʈ ����(G)");
	//�޺�
	char strCombo[20];//�޺��� ��Ÿ����
	sprintf(strCombo, "%d �޺�", nCombo);
	ScreenPrint(44, 13, strCombo);
}



// �ð� ��Ʈ�ѷ�
typedef struct  CONTROL {
	clock_t MovTime; // ��ũ ���ߴ� �̵��ð�
	clock_t OldTime; // ���� �̵��ð�
	int nMagic; // ��ũ ���� ����

}CONTROL;
CONTROL Control;

// ���� ���� �� �غ�ȭ��
void ReadyMap() {
	ScreenPrint(15, 10, "������ ����");
	ScreenPrint(2, 26, "����������  ����������");
	ScreenPrint(2, 27, "  A     S     D       J     K      L");
	// ���� ���� Ű ���� 
}

// Render�Լ����� �����̸鼭 ���
void ReadyMap1() {
	SetColor(10);
	ScreenPrint(10, 15, "Press Enter to Start");
	ScreenPrint(15, 20, "��ũ ����");
	SetColor(15);
}

void ResultMap()
{
	
	ScreenPrint(9, 7, "��-------------------��");
	for (int i = 8; i < 15; i++) {
		ScreenPrint(9, i, "��\t\t     ��");
	}
	SetColor(9); ScreenPrint(15, 10, "GAME END !");SetColor(15);
	char UserScore[20];//����� ������ ��Ÿ��
	sprintf(UserScore, "Score : %d ��", nScore);
	ScreenPrint(14, 12, UserScore);
	ScreenPrint(9, 15, "��-------------------��");

}



string Note[ALLNOTE];
void NoteCheck(void);



// 2���� �迭�� �Ʒ��� �������Բ� ���ִ� �Լ�
void ShowNote(int n) {
	for (int i = 0; i < 27; i++) {
		ScreenPrint(2, 27-i, Note[n+i]);
	}
}



// �Է� Ű �Ǻ����ִ� �Լ�
void CheckKey(int nKey);




void init() {
	Control.MovTime = 52;
	Control.OldTime = 0;
	Control.nMagic = 1;
	Stage = READY;
	
	/*for (int i = 0; i < ALLNOTE; i++) {
		Note[i] = " ";
	}*/ // ��Ʈ �ʱ�ȭ �� �ʿ� �ֳ�? noteCheck ���� ��Ʈ�� ��� �ʱ�ȭ���ֱ⶧����
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
		// ���� ���� �� �ð� ��������
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
	clock_t Curtime = clock(); // ���ݱ��� �帥 �ð�
	ScreenClear();
	//����ڵ�
	Map();
	Hitmap(nkey);
	ScoreMap();
	switch (Stage) {
	case READY ://������
		Oldtime1 = Curtime;
		ReadyMap();
		if (Curtime % 1000 > 500) {
			ReadyMap1();
		}//0.5�� ������ ȭ���� ���
		break;
	case PAUSE:
		return;
	case RUNNING :
		if (RunningTime > 3100) //3�� ���ĺ���
		{
			if (Curtime - Control.OldTime > Control.MovTime) 
			{
				Control.OldTime = Curtime;
				n++;//��Ʈ�� ����� �迭�� �ε����� ����
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
	SoundSystem(); // FMOD ��� �غ�
	ScreenInit();
	init(); // stage�� ready ���·� ����� ��Ʈ�� �ʱ�ȭ
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

				
				Stage = RUNNING; // ���� �Է� �� running���� ���� ȣ��
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

		Update();  // ������ ����
		Render(nKey);  // ȭ�����

		
	}
	Release(); // ����
	ScreenRelease();
	return 0;
}




// �Ǻ�
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
	///////////// L > D > L > S ///// 10�������� 4�� �ݺ� 

	Note[195 + Control.nMagic] = nKeyAJ;  // 14�� ���

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
	Note[274 + Control.nMagic] = nKeyDL;    // 22��

	Note[290 + Control.nMagic] = nKeySK;

	Note[298 + Control.nMagic] = nKeyDL;
	Note[302 + Control.nMagic] = nKeySK;
	Note[306 + Control.nMagic] = nKeyDL;
	Note[310 + Control.nMagic] = nKeySK;
	Note[314 + Control.nMagic] = nKeyAJ;

	//���ӱ���

	for (int i = 0; i < 7; i++)
	{
		Note[318 + i + Control.nMagic] = nKeyAJ;
	}

	Note[330 + Control.nMagic] = nKeyL;
	Note[332 + Control.nMagic] = nKeyK;
	Note[334 + Control.nMagic] = nKeyJ;

	//Note[351 + Control.nMagic] = nKeyA; // 26�� ���

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

// Ű�� ���ڿ� �����͸� ��ȯ���ִ� �Լ�
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

// �浹ó��
// main���� �ش� Ű �Է½� ȣ��Ǵ� �Լ�

void CheckKey(int nKey) {
	string KeyType; // �Է��� Ű�� ����
	KeyType = GetKeyType(nKey);
	if (Note[n] == KeyType) { // Perfect�Ǻ� ������ Note�� �Է��� KeyType�� ��ġ�ϴ� ���
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "��Perfect��");
		
		
	}
	else if ((n > 0 && (Note[n - 1] == KeyType)) || (Note[n + 1] == KeyType)) { // Great �Ǻ� ������ Note�� �Է��� KeyType�� ��ġ�ϴ� ���
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "��Great��");
		
	}
	else {
		nCombo = 0;
	}
}
