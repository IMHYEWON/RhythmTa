#pragma once

#include<time.h>
#include<string>
#include"fmod.hpp"

using namespace std;
using namespace FMOD;

//**********************************************��� ����**********************************************
// Ű�� ���� ����
#define NumOfKey 10
//��Ʈ ����
#define ALLNOTE 1300
// �Է� Ű ������ ����� ����
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
// ����Ű ����� ����
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

//*********************************************����ü&����ü ���� ����**********************************************
// �Է� Ű(inputKey), ��Ʈ ����(nKey) ����ü ����
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KEYNOTE;
KEYNOTE KeyIndex[NumOfKey]; //�Է� Ű&��Ʈ ���� ������ŭ ����ü �迭 ����

// �������� ����
typedef enum _STAGE {
	READY, RUNNING, PAUSE, RESULT, SYNC, END
}STAGE;
STAGE Stage; 

// �ð� ��Ʈ�ѷ�
typedef struct  CONTROL {
	clock_t MovTime; // ��ũ ���ߴ� �̵��ð�
	clock_t OldTime; // ���� �̵��ð�
	int nMagic; // ��ũ ���� ����
}CONTROL;
CONTROL Control;

//*********************************************���� ���� ����**********************************************
// ���� ����
System* pSystem;
Sound* pSound[2];
Channel* pChannel[1];

//�ð�
clock_t RunningTime;
clock_t PauseStart;
clock_t PauseEnd;
clock_t PauseTime = 0;
clock_t SyncStart;
clock_t SyncEnd;
clock_t SyncTime = 0;
clock_t Oldtime1 = 0;
clock_t Oldtime = 0;

// ��Ʈ ������ string���� ����
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

// ��Ʈ �迭
string Note[ALLNOTE];

// ���� perfect ������ Note �迭 �ε��� ����
int curNoteIndex;

// ���� ����
int nScore; //
char strScore[20] = "  ";

// �޺� ����
int nCombo = 0;

// ��ũ ����
int Syncnum = 1;

//*********************************************�Լ� ����**********************************************
//Ű
int isTwoKey(string note);
void CheckKey(string inputKey);
string secondkbhit(int nKey, string inputKeyStr);
string GetKeyType(string nKey);

//��Ʈ
void NoteCheck(void);
void ShowNote();
string HitNote(string inputKey);

//����
void SoundSystem();
void Play(int Sound_num);

//��
void Map(void);
void ScoreMap(void);
void ReadyMap();
void ReadyMap1();
void ResultMap();
void SyncMap();

//�ʱ�ȭ
void init();
void KeyIndexInit();

//�⺻ ���
void Update();
void Render(int nKey);
void Release();
