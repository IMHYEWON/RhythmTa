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

//메인
int main(void) {

	SoundSystem(); // FMOD 사용 준비

	while (Stage != END) {
	
		Play(0); // pSound[0] (=opening.wav)를 실행
		
		ScreenInit(); 
		KeyIndexInit();
		init(); // stage를 ready 상태로 만들고 노트들 초기화
		
		int inputKey=0;
		while (1) {
			if (_kbhit()) {
				inputKey = _getch();
				if (inputKey == '\r') {
					if (Stage == READY) {
						pChannel[0]->stop();
						Play(1); // pSound[0] (=Festival_of_Ghost.wav)를 실행
					}
					else if (Stage == PAUSE) { // 스테이지 Pause 상태일 때 엔터를 누를 경우
						PauseEnd = clock();
						PauseTime += PauseEnd - PauseStart;
						pChannel[0]->setPaused(false); // 현재 pChannel[0]에 있는 노래의 일시 정지를 해제한다.
					}
					else if (Stage == SYNC) // 스테이지가 Sync 상태일 때 엔터를 누를 경우
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

					Stage = RUNNING; // 엔터 입력 시 running시작 음악 호출
				}

				if (inputKey == 'p') {
					if (Stage == RUNNING) {
						PauseStart = clock();
						pChannel[0]->setPaused(true); // 현재 pChannel[0]에 재생중인 노래를 일시 정지한다.
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

					string inputKeyStr; // CheckKey()의 인자로 줄 string 변수 선언 
					inputKeyStr = inputKey; // nKey를 string 변수에 대입 
					if (isTwoKey(Note[curNoteIndex]) || (curNoteIndex > 0 && isTwoKey(Note[curNoteIndex - 1])) || isTwoKey(Note[curNoteIndex + 1])) { //현재 노트가 두 개라면
						inputKeyStr = secondkbhit(inputKey, inputKeyStr); // 첫 번째 키와 비교를 위한 int nKey와 string 반환을 위한 string inputKeyStr을 변수로 줌 
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

					if (inputKey == LEFT)// 싱크 줄이기
					{
						Sync2 = Sync1;
						if (Syncnum <= -30)
							Syncnum = Syncnum;
						else
							Syncnum--;

					}
					else if (inputKey == RIGHT) // 싱크 높이기
					{
						Sync2 = Sync1;
						if (Syncnum >= 30)
							Syncnum = Syncnum;
						else
							Syncnum++;
					}
					else // 다른 키 입력 못받게함
					{
						continue;
					}

					Sync3 = to_string(Syncnum);
					Sync2 += Sync3;
					Control.nMagic = Syncnum;


				}
			}

			Update();  // 데이터 갱신
			Render(inputKey);  // 화면출력


		}
		Release(); // 해제
		ScreenRelease();
	}
	return 0;
}

