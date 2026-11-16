
#pragma once

#include "map.h"
#include "damageObj.h"

class CSpawnPoint;

class CBossRock : public CDamageObj
{
public:
    CBossRock(Point, CMap*, CSpawnPoint*);

    int Action(const ObjList&, ObjList&);
    void Draw();

    // 対象スポナー
    CSpawnPoint* spawn{ nullptr };

private:
    int attackTimer = 0;
    int attackCoolTime = 60;

    // 向き
    int direction{ 1 };

    // アニメーション
    int animationTimer{ 0 };
    int animationFrame{ 0 };
};
