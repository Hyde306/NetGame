#pragma once
#include "NetData.h"

class CMap;


class CClient
{
public:
    //自分のID
    int myPlayerID{ -1 };

    //接続中のプレイヤー情報
    PlayerData PLAYER[MAX_PLAYER];

    //敵の情報
    EnemyData ENEMY[MAX_ENEMY];

    ItemData ITEM[MAX_ITEM];

    //ネットワークハンドル
    int netHandle{ -1 };

    //初回接続
    bool Connect(const IPDATA ip);

    //接続時にプレイヤーIDを受信
    void ReceiveJoin();

    //送信処理
    void Send(const ObjList&);

    //受信処理
    void Receive();
};
