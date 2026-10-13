#pragma once
#include "item.h"

class CMap;

class CItemExp :public CItem
{
public:
	CItemExp(Point, CMap*);

	int Action(const ObjList&, ObjList&);
	void Draw();

};