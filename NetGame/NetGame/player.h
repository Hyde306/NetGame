#pragma once
//#include "objBase.h"
#include "map.h"
#include "damageObj.h"

class CPlayer :public CDamageObj
{
public:
	CPlayer(CMap*);

	

	int Action(const ObjList&, ObjList&);
	void Draw();

	//CMap* map = nullptr;
	
	// HPバー描画
	void DrawHPBar();
	//攻撃中フレーム
	int att_frame{ 0 };

	//経験値
	int exp{ 0 };
	int level{ 1 };
	int nextExp{ 5 };

	void LevelUp();

};