#pragma once
#include "objBase.h"

class CSpawnPoint : public BaseVector
{
public:
    CSpawnPoint(Point, int, CMap*);

    int Action(const ObjList&, ObjList&);
    void Draw();

    // 出現する敵の種類
    int EnemyNo{ 0 };

    // 敵が存在しているか
    bool enemyAlive{ false };

    // ボスが一度でも出現したか
    bool bossSpawned{ false };
};