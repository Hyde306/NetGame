#pragma once
#include "objBase.h"

class CSpawnPoint :public BaseVector
{
public:
	//位置,敵のタイプ,マップ
	CSpawnPoint(Point, int, CMap*);

	int Action(const ObjList&, ObjList&);
	void Draw();

	//出現する敵の種類
	int EnemyNo{ 0 };
	//出現敵が存在しているかフラグ
	bool enemyAlive{ false };
};