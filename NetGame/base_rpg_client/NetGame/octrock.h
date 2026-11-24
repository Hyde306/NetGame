//octrock.h
#pragma once
//#include "objBase.h"
#include "map.h"
#include "damageObj.h"

class CSpawnPoint;

class COctrock :public CDamageObj,public CEnemyData
{
public:
	COctrock(Point,CMap*, int);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//対象スポナー
	CSpawnPoint* spawn{ nullptr };

	int attackTimer = 0;
	//CMap* map = nullptr;
};