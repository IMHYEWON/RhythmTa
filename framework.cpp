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
#include"Note.h"
#include"Note.cpp"

using namespace std;
using namespace FMOD;

//����
int main(void) {

	SoundSystem(); // FMOD ��� �غ�

	while (Stage != END) {
	
		Play(0); // pSound[0] (=opening.wav)�� ����
		
		ScreenInit(); 
		KeyIndexInit();
		init(); // stage�� ready ���·� ����� ��Ʈ�� �ʱ�ȭ
		
		int inputKey=0;
		while (1) {
			if (_kbhit()) {
				inputKey = _getch();
				if (inputKey == '\r') {
					if (Stage == READY) {
						pChannel[0]->stop();
						Play(1); // pSound[0] (=Festival_of_Ghost.wav)�� ����
					}
					else if (Stage == PAUSE) { // �������� Pause ������ �� ���͸� ���� ���
						PauseEnd = clock();
						PauseTime += PauseEnd - PauseStart;
						pChannel[0]->setPaused(false); // ���� pChannel[0]�� �ִ� �뷡�� �Ͻ� ������ �����Ѵ�.
					}
					else if (Stage == SYNC) // ���������� Sync ������ �� ���͸� ���� ���
					{
						NoteCheck();
						pChannel[0]->stop();
						Play(1);
						SyncEnd = clock();
						SyncTime += SyncEnd - SyncStart;
					}
					else if (Stage == RESULT) {
						break;
					}

					Stage = RUNNING; // ���� �Է� �� running���� ���� ȣ��
				}

				if (inputKey == 'p') {
					if (Stage == RUNNING) {
						PauseStart = clock();
						pChannel[0]->setPaused(true); // ���� pChannel[0]�� ������� �뷡�� �Ͻ� �����Ѵ�.
						Stage = PAUSE;
					}
				}
				if (Stage == READY && inputKey == 'c')
				{
					SyncStart = clock();
					Stage = SYNC;
					SyncMap();
				}

				if (inputKey == 'a' || inputKey == 's' || inputKey == 'd' || inputKey == 'j' || inputKey == 'k' || inputKey == 'l') {
					if (Stage == PAUSE) { continue; }

					string inputKeyStr; // CheckKey()�� ���ڷ� �� string ���� ���� 
					inputKeyStr = inputKey; // nKey�� string ������ ���� 
					if (isTwoKey(Note[curNoteIndex]) || (curNoteIndex > 0 && isTwoKey(Note[curNoteIndex - 1])) || isTwoKey(Note[curNoteIndex + 1])) { //���� ��Ʈ�� �� �����
						inputKeyStr = secondkbhit(inputKey, inputKeyStr); // ù ��° Ű�� �񱳸� ���� int nKey�� string ��ȯ�� ���� string inputKeyStr�� ������ �� 
					}
					CheckKey(inputKeyStr);
				}

				if (Stage == RESULT) {
					if (inputKey == 'q') {
						ScreenClear();
						return 0;
					}
				}

				if (Stage == SYNC)
				{

					if (inputKey == LEFT)// ��ũ ���̱�
					{
						Sync2 = Sync1;
						if (Syncnum <= -30)
							Syncnum = Syncnum;
						else
							Syncnum--;

					}
					else if (inputKey == RIGHT) // ��ũ ���̱�
					{
						Sync2 = Sync1;
						if (Syncnum >= 30)
							Syncnum = Syncnum;
						else
							Syncnum++;
					}
					else // �ٸ� Ű �Է� ���ް���
					{
						continue;
					}

					Sync3 = to_string(Syncnum);
					Sync2 += Sync3;
					Control.nMagic = Syncnum;


				}
			}

			Update();  // ������ ����
			Render(inputKey);  // ȭ�����


		}
		Release(); // ����
		ScreenRelease();
	}
	return 0;
}

