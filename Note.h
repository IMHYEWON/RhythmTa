#pragma once
#include<string>
using namespace std;

// �Է� Ű ������ ����� ����
#define NumOfKey 10
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

// �Է� Ű(inputKey)�� ��Ʈ ����(nKey)�� ����ü�� ����
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KeyNote;

//�Է� Ű&��Ʈ ���� ������ŭ ����ü �迭 ����
KeyNote KeyIndex[NumOfKey]; 