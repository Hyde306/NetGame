//octrock.h
#pragma once
//#include "objBase.h"
#include "map.h"
#include "damageObj.h"

class CSpawnPoint;

class COctrock :public CDamageObj
{
public:
	COctrock(Point,CMap*, CSpawnPoint*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//対象スポナー
	CSpawnPoint* spawn{ nullptr };

	//CMap* map = nullptr;
};