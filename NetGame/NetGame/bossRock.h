
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
};
