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



//���� �Լ�
void SoundSystem() {
	System_Create(&pSystem);

	pSystem->init(4, FMOD_INIT_NORMAL, NULL);

	pSystem->createSound("opening.wav", FMOD_LOOP_NORMAL | FMOD_HARDWARE, NULL, &pSound[0]); // ����������
	pSystem->createSound("Festival_of_Ghost.wav", FMOD_DEFAULT, NULL, &pSound[1]); // ��������
}
void Play(int Sound_num) {

	pSystem->playSound(FMOD_CHANNEL_FREE, pSound[Sound_num], 0, pChannel);

}

// �� �Լ�
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
// ���� ���� �� �غ�ȭ��
void ReadyMap() {
	ScreenPrint(15, 10, "������ ����");
	ScreenPrint(2, 26, "����������  ����������");
	ScreenPrint(11, 20, "Press c to Syncmap");
	ScreenPrint(2, 27, "  A     S     D       J     K      L");
	// ���� ���� Ű ���� 
}
// Render�Լ����� �����̸鼭 ���
void ReadyMap1() {
	SetColor(10);
	ScreenPrint(10, 15, "Press Enter to Start");

	SetColor(15);
}
// ���ȭ�� ���
void ResultMap()
{

	ScreenPrint(9, 7, "��---------------------��");
	for (int i = 8; i < 15; i++) {
		ScreenPrint(9, i, "��\t\t       ��");
	}
	SetColor(9); ScreenPrint(15, 10, "GAME END !"); SetColor(15);
	char UserScore[20];//����� ������ ��Ÿ��
	sprintf(UserScore, "Score : %d ��", nScore);
	ScreenPrint(13, 12, UserScore);
	ScreenPrint(9, 15, "��---------------------��");
	SetColor(10);
	ScreenPrint(10, 18, "Press Enter to Restart");
	SetColor(14);
	ScreenPrint(13, 20, "Press q to Exit");
	SetColor(15);
}
// ��ũȭ�� ���
void SyncMap()
{
	ScreenPrint(4, 15, "Press �� �� to tune the Sync! : "+to_string(Syncnum));

	ScreenPrint(10, 20, "Press Enter to Start ");
}

// �ʱ�ȭ �Լ�
//�ʱ� �ð��� ��Ʈ�� �׸��� ���ھ� �ʱ�ȭ
void init() {
	curNoteIndex = 0; 
	nScore = 0; //
	Control.MovTime = 52;
	Control.OldTime = 0;
	Control.nMagic = 1;
	Stage = READY;

	RunningTime = 0;
	NoteInit();
	SyncTime = 0;
	PauseTime = 0;
}
//Ű�� ��Ʈ string�� KeyNote����ü�� �ʱ�ȭ �����ִ� �Լ�
void KeyIndexInit() {
	//index�� �Է¹��� Ű�� ���� (note.h�� ����� ����)
	KeyIndex[none].inputKey = "none";
	KeyIndex[none].nKey = "                                      ";
	KeyIndex[a].inputKey = "a";
	KeyIndex[a].nKey = "����";
	KeyIndex[s].inputKey = "s";
	KeyIndex[s].nKey = "      ����";
	KeyIndex[d].inputKey = "d";
	KeyIndex[d].nKey = "            ����";
	KeyIndex[j].inputKey = "j";
	KeyIndex[j].nKey = "                    ����";
	KeyIndex[k].inputKey = "k";
	KeyIndex[k].nKey = "                          ����";
	KeyIndex[l].inputKey = "l";
	KeyIndex[l].nKey = "                                ����";
	KeyIndex[aj].inputKey = "aj";
	KeyIndex[aj].nKey = "����              ����";
	KeyIndex[ks].inputKey = "ks";
	KeyIndex[ks].nKey = "      ����              ����";

	KeyIndex[dl].inputKey = "dl";
	KeyIndex[dl].nKey = "            ����              ����";
}
// �Ǻ�
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

	for (int i = 0; i < 7; i += 2)
	{
		Note[318 + i + Control.nMagic] = nKeyAJ;
	}

	Note[330 + Control.nMagic] = nKeyL;
	Note[332 + Control.nMagic] = nKeyK;
	Note[334 + Control.nMagic] = nKeyJ;

	//Note[351 + Control.nMagic] = nKeyA; // 26�� ���

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
// �⺻ ��� �Լ�
void Update() {
	clock_t Curtime = clock();
	switch (Stage) {
	case READY:
		Oldtime = Curtime;
		break;
	case RUNNING:
		// ���� ���� �� �ð� ��������
		RunningTime = Curtime - Oldtime - PauseTime - SyncTime;

		break;
	case PAUSE:
		break;
	}
}
void Render(int nkey) {
	clock_t Curtime = clock(); // ���ݱ��� �帥 �ð�
	ScreenClear();
	//����ڵ�

	Map();
	ScoreMap();
	if (Stage == SYNC)
	{
		SyncMap();
	}
	switch (Stage) {
	case READY://������
		Oldtime1 = Curtime;
		ReadyMap();
		if (Curtime % 1000 > 500) {
			ReadyMap1();
		}//0.5�� ������ ȭ���� ���
		break;
	case PAUSE:
		return;
	case RUNNING:
		if (RunningTime > 3100) //3�� ���ĺ���
		{
			if (Curtime - Control.OldTime > Control.MovTime)
			{
				Control.OldTime = Curtime;
				curNoteIndex++;//��Ʈ�� ����� �迭�� �ε����� ����
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

// Ű �Լ�
// �� ��° Ű �Է��� �޴� �Լ�
string secondkbhit(int inputKey, string inputKeyStr) {
	int inputKey2; // �� ��°�� �Էµ� Ű�� �ޱ� ���� ����
	for (int i = 0; i < 1000; i++) {
		if (_kbhit()) {
			inputKey2 = _getch();
			if (inputKey2 == 'a' || inputKey2 == 's' || inputKey2 == 'd' || inputKey2 == 'j' || inputKey2 == 'k' || inputKey2 == 'l') {
				if (inputKey == inputKey2) { //���� Ű�� ��� ������ ��� �ν��ϴ� ���� ����
					continue;
				}
				else if (inputKey < inputKey2) { //���ĺ� ������ ���� ���� inputKeyStr�� ���� ��ġ
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
// Ű�� �ش��ϴ� ��Ʈ ���ڿ��� ��ȯ���ִ� �Լ�
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
// ���� ��Ʈ�� �� ���� ��Ʈ���� Ȯ�����ִ� �Լ�
int isTwoKey(string nKeyStr) {
	for (int i = 0; i < NumOfKey; i++) {
		if (nKeyStr == KeyIndex[i].nKey) { //note�� KeyIndex ����ü�� nKey string�� ���ٸ�
			if (i >= aj) { //��� aj���� ũ�ٸ� �� ���� ��Ʈ�� ������ �����Ƿ� 1 ����
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	return 0;
}
// �浹ó��
void CheckKey(string inputKeyStr) {
	string nKeyStr; // �Է��� Ű�� ����
	nKeyStr = GetKeyType(inputKeyStr);
	if (Note[curNoteIndex] == nKeyStr) { // Perfect�Ǻ� ������ Note�� �Է��� KeyType�� ��ġ�ϴ� ���
		nScore += 500;
		nCombo++;
		Note[curNoteIndex] = HitNote(nKeyStr);
		sprintf(strScore, "%s", "��Perfect��");
	}
	else if ((curNoteIndex > 0 && (Note[curNoteIndex - 1] == nKeyStr)) || (Note[curNoteIndex + 1] == nKeyStr)) { // Great �Ǻ� ������ Note�� �Է��� KeyType�� ��ġ�ϴ� ���
		nScore += 300;
		nCombo++;
		Note[curNoteIndex + 1] = HitNote(nKeyStr);
		Note[curNoteIndex - 1] = HitNote(nKeyStr);
		sprintf(strScore, "%s", "��Great��");
	}
	else {
		nCombo = 0;
	}
}

// ��Ʈ �Լ�
// 2���� �迭�� �Ʒ��� �������Բ� ���ִ� �Լ�
void ShowNote() {
	for (int i = 0; i < 27; i++) {
		if (28 - i >= 27) { // ��Ʈ ���� ������ ������ ��� ���δ� ��������� ����.
			SetColor(14);
		}
		else SetColor(15);
		ScreenPrint(2, 28 - i, Note[curNoteIndex + i]);
	}
}
// ��Ʈ�� ������ ��Ʈ�� ��Ʈ�� ��Ʈ string���� ��ȯ
string HitNote(string inputKey) {
	for (int i = 0; i < inputKey.length(); i++) {
		if (inputKey[i] != ' ') {
			inputKey[i] = 'oo';
		}
	}
	return inputKey;
}

// ��ũ �Լ�
void ControlSync(int inputKey) {
	if (inputKey == LEFT) { // ��ũ ���̱�
		if (Syncnum <= -30) {
			Syncnum = Syncnum;
		}
		else {
			Syncnum--;
		}
	}
	else  { // ��ũ ���̱�
		if (Syncnum >= 30) {
			Syncnum = Syncnum;
		}
		else {
			Syncnum++;
		}
	}
	Control.nMagic = Syncnum;
}