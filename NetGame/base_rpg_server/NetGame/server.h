//サーバークラス
#pragma once
#include "NetData.h"

//接続中のプレイヤー情報
class CPlayerInfo
{
public:
    //クライアントとの通信に使用するネットワークハンドル
    int netHandle{ -1 };

    //接続しているプレイヤーのデータ
    PlayerData playerData;
};

class CServer {
public:
    //ネットワークハンドル
    int NetHandle{ -1 };

    //接続中のプレイヤー情報
    CPlayerInfo PLAYER[4];

    //接続処理(ポート番号)
    bool Accept(int);

    //接続してきたプレイヤーを登録する
    void New_Receive(ObjList&, int, CMap*);

    //クライアントからデータ受信
    void Receive();
};
