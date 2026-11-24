#pragma once
#include "damageObj.h"
#include "map.h"
#include "NetData.h"

class CNetPlayer :public CDamageObj
{
public:
    //対応するプレイヤーのPlayerID
    int netID{ -1 };

    CNetPlayer(int, PlayerData*, CMap*);

    int Action(const ObjList&, ObjList&);
    void Draw();

    //通信で受信したプレイヤーデータ
    PlayerData* data{ nullptr };

    //アタックカウンター
    int old_attackCount{ 0 };
};
