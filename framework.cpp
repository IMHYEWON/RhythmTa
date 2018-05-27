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
		int nKey = 0;
		ScreenInit();
		KeyIndexInit();
		init(); // stage�� ready ���·� ����� ��Ʈ�� �ʱ�ȭ
		Play(0); // pSound[0] (=opening.wav)�� ����

		while (1) {
			if (_kbhit()) {
				nKey = _getch();
				if (nKey == '\r') {
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

				if (nKey == 'p') {
					if (Stage == RUNNING) {
						PauseStart = clock();
						pChannel[0]->setPaused(true); // ���� pChannel[0]�� ������� �뷡�� �Ͻ� �����Ѵ�.
						Stage = PAUSE;
					}
				}
				if (Stage == READY && nKey == 'c')
				{
					SyncStart = clock();
					Stage = SYNC;
					SyncMap();
				}

				if (nKey == 'a' || nKey == 's' || nKey == 'd' || nKey == 'j' || nKey == 'k' || nKey == 'l') {
					if (Stage == PAUSE) { continue; }

					string inputKeyStr; // CheckKey()�� ���ڷ� �� string ���� ���� 
					inputKeyStr = nKey; // nKey�� string ������ ���� 
					if (isTwoKey(Note[n]) || (n > 0 && isTwoKey(Note[n - 1])) || isTwoKey(Note[n + 1])) { //���� ��Ʈ�� �� �����
						inputKeyStr = secondkbhit(nKey, inputKeyStr); // ù ��° Ű�� �񱳸� ���� int nKey�� string ��ȯ�� ���� string inputKeyStr�� ������ �� 
					}
					CheckKey(inputKeyStr);
				}

				if (Stage == RESULT) {
					if (nKey == 'q') {
						ScreenClear();
						return 0;
					}
				}

				if (Stage == SYNC)
				{

					if (nKey == LEFT)// ��ũ ���̱�
					{
						Sync2 = Sync1;
						if (Syncnum <= -30)
							Syncnum = Syncnum;
						else
							Syncnum--;

					}
					else if (nKey == RIGHT) // ��ũ ���̱�
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
			Render(nKey);  // ȭ�����


		}
		Release(); // ����
		ScreenRelease();
	}
	return 0;
}

