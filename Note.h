#pragma once
#include<string>
using namespace std;
// ��Ʈ�� �ش��ϴ� ���� ����
typedef struct KeyNote {
	string inputKey;
	string nKey;
}KeyNote;

KeyNote KeyIndex[10];
KeyIndex[0].inputKey = "none";

/*
	{"a","����" },
	{"s","      ����" },
	{"d","            ����" },
	{"j", "                    ����" },
	{"k","                          ����" },
	{"l","                                ����" },
	{"aj","����              ����" },
	{"sk","      ����              ����" },
	{"dl","            ����              ����" }
};*/
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