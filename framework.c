#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include"Screen.h""
#include "Note.h"
#include<memory.h>

#define ALLNOTE 1000
//
// �Ҹ� ��� PlaySound�Լ�
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")
//PlaySound(TEXT("Festival_of_Ghost.wav"), NULL, SND_ASYNC | SND_LOOP);
//
int n = 0;

int nScore = 0;
char strScore[20] = "  ";
int nCombo = 0;
//
clock_t RunningTime;

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
char* nKeyNone = "                                      ";
char* nKeyL = "                                ����";
char* nKeyK = "                          ����";
char* nKeyJ = "                    ����";
char* nKeyD = "            ����";
char* nKeyS = "      ����";
char* nKeyA = "����";
char* nKeyAJ = "����              ����";
char* nKeySK = "      ����              ����";
char* nKeyDL = "            ����              ����";


// 19�� / 3 / 3 / 3 / 1 / 3 / 3 / 3
char* Note[ALLNOTE];
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
	Control.nMagic = 0;
	Stage = READY;
	NoteCheck();
	for (int i = 0; i < ALLNOTE; i++) {
		Note[i] = " ";
	}
	RunningTime = 0;

	Count.nXofA = 2;   //(2,29)
	Count.nXofS = 8;
	Count.nXofD = 14;
	Count.nXofJ = 21;
	Count.nXofK = 27;
	Count.nXofL = 33;

	Stage = READY;

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
		RunningTime = clock() - Oldtime;
		break;
	case PAUSE:
		break;
	}
	NoteCheck();
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
	//case PAUSE: ���� �������� ���� ���Ŀ� �����ؾߵ�
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
	ScreenInit();
	init(); // �ʱ�ȭ
	PlaySound(TEXT("opening.wav"), NULL, SND_ASYNC | SND_LOOP);
	while (1) {
		
		if (_kbhit()) {
			nKey = _getch();
			if (nKey == '\r') {
				Stage = RUNNING; // ���� �Է� �� running���� ���� ȣ��
				PlaySound(TEXT("Festival_of_Ghost.wav"), NULL, SND_ASYNC | SND_LOOP);
			}
			if (nKey == 'p') {
				Stage = PAUSE;
			}
			if (nKey == 'a' || nKey == 's' || nKey == 'd' || nKey == 'j' || nKey == 'k' || nKey == 'l') {
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
	Note[30+Control.nMagic] = nKeyL;
	Note[40 + Control.nMagic] = nKeyD;
	Note[50 + Control.nMagic] = nKeyL;
	Note[60 + Control.nMagic] = nKeyS;
	Note[70 + Control.nMagic] = nKeyL;
	Note[80 + Control.nMagic] = nKeyD;
	Note[90 + Control.nMagic] = nKeyL;
	Note[100 + Control.nMagic] = nKeyS;
	Note[110 + Control.nMagic] = nKeyL;
	Note[120 + Control.nMagic] = nKeyD;
	Note[130 + Control.nMagic] = nKeyL;
	Note[140 + Control.nMagic] = nKeyS;
	Note[153 + Control.nMagic] = nKeyL;
	Note[163 + Control.nMagic] = nKeyD;
	Note[173 + Control.nMagic] = nKeyL;
	Note[183 + Control.nMagic] = nKeyS;
	Note[195 + Control.nMagic] =  nKeyAJ;   // 14�� ���
	Note[210 + Control.nMagic] = nKeySK;
	Note[215 + Control.nMagic] = nKeyDL;
	Note[220 + Control.nMagic] = nKeySK;
	Note[225 + Control.nMagic] = nKeyAJ;
	Note[230 + Control.nMagic] = nKeySK;
	Note[235 + Control.nMagic] = nKeyDL;
	Note[253 + Control.nMagic] = nKeySK;
	Note[255 + Control.nMagic] = nKeyAJ;

	Note[268 + Control.nMagic] = nKeyDL;
	Note[272 + Control.nMagic] = nKeySK;
	Note[276 + Control.nMagic] = nKeyAJ;
	Note[280 + Control.nMagic] = nKeyD;    // 22��

	Note[297 + Control.nMagic] = nKeySK;
	Note[304 + Control.nMagic] = nKeyDL;
	Note[308 + Control.nMagic] = nKeySK;
	Note[312 + Control.nMagic] = nKeyDL;
	Note[316 + Control.nMagic] = nKeySK;
	Note[320 + Control.nMagic] = nKeyAJ;

	Note[338 + Control.nMagic] = nKeyL;
	Note[340 + Control.nMagic] = nKeyK;
	Note[342 + Control.nMagic] = nKeyJ;
	Note[351 + Control.nMagic] = nKeyA; // 26�� ���
	
	Note[362 + Control.nMagic] = nKeyS;
	Note[367 + Control.nMagic] = nKeyDL;
	Note[374 + Control.nMagic] = nKeyS;
	Note[379 + Control.nMagic] = nKeyDL;
	Note[386 + Control.nMagic] = nKeyS;
	Note[391 + Control.nMagic] = nKeyDL;
	Note[398 + Control.nMagic] = nKeyS;
	Note[402 + Control.nMagic] = nKeyA;
	// 406 +42
	Note[362+42 + Control.nMagic] = nKeyS;
	Note[367 + 42 + Control.nMagic] = nKeyDL;
	Note[374 + 42 + Control.nMagic] = nKeyS;
	Note[379 + 42 + Control.nMagic] = nKeyDL;
	Note[386 + 42 + Control.nMagic] = nKeyS;
	Note[391 + 42 + Control.nMagic] = nKeyDL;
	Note[398 + 44 + Control.nMagic] = nKeyS;
	Note[402 + 48 + Control.nMagic] = nKeyA;

	Note[464 + Control.nMagic] = nKeyJ;
	Note[465 + Control.nMagic] = nKeyK;
	Note[466 + Control.nMagic] = nKeyL;
	Note[478 + Control.nMagic] = nKeyA;
}

// Ű�� ���ڿ� �����͸� ��ȯ���ִ� �Լ�
char *GetKeyType(int nKey) {
	char *KeyType;
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
	char *KeyType; // �Է��� Ű�� ����
	KeyType = GetKeyType(nKey);
	if (strcmp(Note[n], KeyType) == 0) { // Perfect�Ǻ� ������ Note�� �Է��� KeyType�� ��ġ�ϴ� ���
		nScore += 500;
		nCombo++;
		sprintf(strScore, "%s", "��Perfect��");
	}
	else if ((n > 0 && strcmp(Note[n - 1], KeyType) == 0) || strcmp(Note[n + 1], KeyType) == 0) { // Great �Ǻ� ������ Note�� �Է��� KeyType�� ��ġ�ϴ� ���
		nScore += 300;
		nCombo++;
		sprintf(strScore, "%s", "��Great��");
	}
	else {
		nCombo = 0;
	}
}
