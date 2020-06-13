#pragma once
#include "ProcFuncs.h"
class Entity
{
public:
	char pad_0000[284]; //0x0000
	Vector3 pos; //0x011C
	char pad_0128[92]; //0x0128
	int32_t health; //0x0184
	int32_t healthMax; //0x0188
	char pad_018C[800]; //0x018C
}; //Size: 0x04AC


extern Entity* ents[100];
extern Entity* entsptr;