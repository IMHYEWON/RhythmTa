#pragma once
#include<string>
using namespace std;
// ��Ʈ�� �ش��ϴ� ���� ����
#define NumOfKey 10
#define none 0
#define a 1
#define s 2
#define d 3
#define j 4
#define k 5
#define l 6
#define aj 7
#define sk 8
#define dl 9
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KeyNote;
KeyNote KeyIndex[NumOfKey]; //��Ʈ ������ŭ ����ü ����

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