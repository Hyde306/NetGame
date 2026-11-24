//サーバークラス
#pragma once
#include "NetData.h"
#include <set>

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

    //クライアントの切断チェック
    void LostPlayer();

    //切断したプレイヤーのスロット開放処理
    void ReleasePlayer();

    //サーバが管理している情報を接続中の全クライアントへ送信
    void SendAll(const ObjList&);

    //敵情報収集
    void SetEnemyData(const ObjList&, CNetDataAll&);

    //現在使用中の敵ID
    std::set<int> aliveEnemy;

    //管理用敵IDの発行
    int GetNextEnemyID();

    void SetItemData(const ObjList&, CNetDataAll&);
    std::set<int> aliveItem;
    int GetNextItemID();
};
