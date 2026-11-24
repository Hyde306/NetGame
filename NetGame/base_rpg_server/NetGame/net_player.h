#pragma once
#include "damageObj.h"
#include "map.h"
#include "NetData.h"

class CNetPlayer :public CDamageObj
{
public:
    CNetPlayer(PlayerData*, CMap*);

    int Action(const ObjList&, ObjList&);
    void Draw();

    //通信で受信したプレイヤーデータを参照
    PlayerData* data{ nullptr };

    //アタックカウンター
    int old_attackCount{ 0 };

    //経験値
    int exp{ 0 };
};
