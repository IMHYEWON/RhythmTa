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
//
clock_t RunningTime;
clock_t PauseStart;
clock_t PauseEnd;
clock_t PauseTime = 0;

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
	READY, RUNNING,PAUSE, RESULT
}STAGE;
STAGE Stage;



// �������� �⺻ Ʋ
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "����������������������");
	for (int i = 1; i < 29; i++) {
		ScreenPrint(0, i, "��\t\t\t\t\t��");
	}
	ScreenPrint(0, 29, "����������������������");
	ScreenPrint(2, 26, "______________________________________");
}

// ���� ���� ���Ʋ
void ScoreMap() {
	// ����ð�
	char nTime[20];//��� �ð��� ��Ÿ����
	sprintf(nTime, "�ð� : %d.%d��", RunningTime / 1000, RunningTime % 1000);
	ScreenPrint(44, 2, nTime);
	// ���� ���
	ScreenPrint(44, 10, strScore);//Great,Perfect�Ǻ�
	ScreenPrint(44, 22, "Great : 300��");
	ScreenPrint(44, 23, "Perfect : 500��");
	//ScreenPrint(44, 25, "\'p\' to Pause");
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
	SetColor(15);
}



// ��Ʈ�� �ش��ϴ� ���� ����
string nKeyNone = "                                      ";
string nKeyL = "                                ����";
string nKeyK = "                          ����";
string nKeyJ = "                    ����";
string nKeyD = "            ����";
string nKeyS = "      ����";
string nKeyA = "����";
string nKeyAJ = "����              ����";
string nKeySK = "      ����              ����";
string nKeyDL = "            ����              ����";


// 19�� / 3 / 3 / 3 / 1 / 3 / 3 / 3
string Note[ALLNOTE];
void NoteCheck(void);



// 2���� �迭�� �Ʒ��� �������Բ� ���ִ� �Լ�
void ShowNote(int n) {
	for (int i = 0; i < 28; i++) {
		ScreenPrint(2, 28-i, Note[n+i]);
	}
}



// �Է� Ű �Ǻ����ִ� �Լ�
void CheckKey(int nKey);




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
		// ���� ���� �� �ð� ��������
		RunningTime = Curtime - Oldtime - PauseTime;
		break;
	case PAUSE:
		break;
	}
	//NoteCheck();
}





clock_t Oldtime1 = 0;
void Render() {
	clock_t Curtime = clock(); // ���ݱ��� �帥 �ð�
	ScreenClear();
	//����ڵ�
	Map();
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
	}






	ScreenFlipping();
}

void Release() {

}
int main(void) {
	int nKey;
	SoundSystem(); // FMOD ��� �غ�
	ScreenInit();
	init(); // �ʱ�ȭ
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
			if (nKey == 'a' || nKey == 's' || nKey == 'd' || nKey == 'j' || nKey == 'k' || nKey == 'l') {
				if (Stage == PAUSE) continue;
				CheckKey(nKey);
			}
		}

		Update();  // ������ ����
		Render();  // ȭ�����

		
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
	Note[278 + Control.nMagic] = nKeyDL;    // 22��

	Note[295 + Control.nMagic] = nKeySK;

	Note[302 + Control.nMagic] = nKeyDL;
	Note[306 + Control.nMagic] = nKeySK;
	Note[310 + Control.nMagic] = nKeyDL;
	Note[314 + Control.nMagic] = nKeySK;
	Note[318 + Control.nMagic] = nKeyAJ;

	//���ӱ���

	for (int i = 0; i < 7; i++)
	{
		Note[320 + i + Control.nMagic] = nKeyAJ;
	}

	Note[334 + Control.nMagic] = nKeyL;
	Note[336 + Control.nMagic] = nKeyK;
	Note[338 + Control.nMagic] = nKeyJ;

	//Note[351 + Control.nMagic] = nKeyA; // 26�� ���

	Note[353 + Control.nMagic] = nKeyS; //S
	Note[358 + Control.nMagic] = nKeyDL;
	Note[365 + Control.nMagic] = nKeyS; //S
	Note[370 + Control.nMagic] = nKeyDL;
	Note[377 + Control.nMagic] = nKeyS;
	Note[382 + Control.nMagic] = nKeyDL;
	Note[389 + Control.nMagic] = nKeyS;
	//Note[402 + Control.nMagic] = nKeyA;
	// 406 +42
	Note[400 + Control.nMagic] = nKeyS;
	Note[404 + Control.nMagic] = nKeyDL;
	Note[409 + Control.nMagic] = nKeyS;
	Note[413 + Control.nMagic] = nKeyDL;
	Note[418 + Control.nMagic] = nKeyS;
	Note[422 + Control.nMagic] = nKeyDL;
	//Note[398 + 42 + Control.nMagic] = nKeyS;

	for (int i = 0; i <= 10; i++)
	{
		Note[440 + i + Control.nMagic] = nKeyA;
	}


	Note[461 + Control.nMagic] = nKeyJ;
	Note[462 + Control.nMagic] = nKeyK;
	Note[463 + Control.nMagic] = nKeyL;

	Note[470 + Control.nMagic] = nKeyA;

	//

	Note[477 + Control.nMagic] = nKeyJ;
	Note[480 + Control.nMagic] = nKeyK;
	Note[483 + Control.nMagic] = nKeyL;

	Note[491 + Control.nMagic] = nKeyA;
	Note[496 + Control.nMagic] = nKeyDL; //���� D����, AD�������ٵ�


	Note[501 + Control.nMagic] = nKeyJ;
	Note[504 + Control.nMagic] = nKeyK;
	Note[507 + Control.nMagic] = nKeyJ;
	Note[510 + Control.nMagic] = nKeyK;

	Note[514 + Control.nMagic] = nKeyA;
	Note[517 + Control.nMagic] = nKeyD;


	Note[523 + Control.nMagic] = nKeyJ;
	Note[527 + Control.nMagic] = nKeyK;
	Note[531 + Control.nMagic] = nKeyL;

	Note[536 + Control.nMagic] = nKeyA;
	Note[540 + Control.nMagic] = nKeyD;

	Note[543 + Control.nMagic] = nKeyJ;
	Note[545 + Control.nMagic] = nKeyK;
	Note[547 + Control.nMagic] = nKeyL;
	Note[549 + Control.nMagic] = nKeyJ;
	Note[551 + Control.nMagic] = nKeyK;
	Note[553 + Control.nMagic] = nKeyL;

	Note[558 + Control.nMagic] = nKeyA;
	Note[563 + Control.nMagic] = nKeyD;

	//
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

	//Note[630 + Control.nMagic] = nKeyD;
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


	Note[669 + Control.nMagic] = nKeyJ;
	Note[672 + Control.nMagic] = nKeyK;
	Note[675 + Control.nMagic] = nKeyJ;
	Note[678 + Control.nMagic] = nKeyK;

	Note[682 + Control.nMagic] = nKeyA;
	Note[685 + Control.nMagic] = nKeyD;


	Note[691 + Control.nMagic] = nKeyJ;
	Note[695 + Control.nMagic] = nKeyK;
	Note[699 + Control.nMagic] = nKeyL;

	Note[704 + Control.nMagic] = nKeyA;
	Note[708 + Control.nMagic] = nKeyD;

	Note[711 + Control.nMagic] = nKeyJ;
	Note[713 + Control.nMagic] = nKeyK;
	Note[715 + Control.nMagic] = nKeyL;
	Note[717 + Control.nMagic] = nKeyJ;
	Note[719 + Control.nMagic] = nKeyK;
	Note[721 + Control.nMagic] = nKeyL;

	//
	Note[726 + Control.nMagic] = nKeyJ;
	Note[729 + Control.nMagic] = nKeyK;
	Note[732 + Control.nMagic] = nKeyL;

	Note[737 + Control.nMagic] = nKeyA;
	Note[740 + Control.nMagic] = nKeyD;


	Note[746 + Control.nMagic] = nKeyJ;
	Note[749 + Control.nMagic] = nKeyK;
	Note[752 + Control.nMagic] = nKeyJ;
	Note[755 + Control.nMagic] = nKeyK;

	Note[760 + Control.nMagic] = nKeyA;
	Note[765 + Control.nMagic] = nKeyD;

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
