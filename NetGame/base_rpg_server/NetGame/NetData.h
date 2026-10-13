// NetData.h
#pragma once
#include "objBase.h"

//-----------------------------------------------------
// 通信するプレイヤーデータ
//-----------------------------------------------------
struct PlayerData {
    Point pos;
};

struct PlayerSendData {
    //プレイヤー座標
    Point pos;
};
