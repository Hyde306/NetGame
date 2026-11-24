#include "client.h"
#include "player.h"
#include "library_function.h"
#include <cstring>

//初回接続
bool CClient::Connect(const IPDATA ip)
{
    netHandle = ConnectNetWork(ip, 40);//40...ポート番号
    if (netHandle != -1)
    {
        ReceiveJoin();//ID受信処理
        return true;
    }
    return false;
}

//PlayerID受信処理
void CClient::ReceiveJoin()
{
    while (true)
    {
        if (GetNetWorkDataLength(netHandle) > 0)
        {
            char str[512]{};

            //データ受信
            NetWorkRecv(netHandle, str, sizeof(CJoinData));
            CJoinData join;
            memcpy_s(&join, sizeof(join), str, sizeof(join));
            //受信データをIDにコピー
            myPlayerID = join.PlayerID;
            break;
        }
    }
}

//-----------------------------------------------------
// プレイヤーデータをサーバーへ送信
//-----------------------------------------------------
void CClient::Send(const ObjList& base)
{
    //Playerオブジェクトを取得
    CPlayer* player = dynamic_cast<CPlayer*>(Get_obj(base, (int)ObjID::PLAYER));

    //====================================
    // 送信データの準備
    //====================================
    char str[512]{ 0 };//ネットワーク送信用のバッファ
    PlayerSendData data{ };//送信データ

    if (player) {
        //プレイヤーの位置をコピー
        data.pos = player->pos;
        //今後、向きや攻撃情報などもここでコピー
        data.dir = player->Dir;
        data.attackCount = player->attackCount;
        data.alive = true;
    }

    //構造体のデータを送信用バッファへコピー
    memcpy_s(str, sizeof(PlayerSendData), &data, sizeof(PlayerSendData));

    //サーバーへデータを送信
    NetWorkSend(netHandle, str, sizeof(PlayerSendData));
}

//データ受信
void CClient::Receive()
{
    //データが送られてきているかチェック
    if (GetNetWorkDataLength(netHandle) >= (int)sizeof(CNetDataAll)) {
        char str[sizeof(CNetDataAll)];
        CNetDataAll data;
        //データ受信
        NetWorkRecv(netHandle, str, sizeof(CNetDataAll));
        //処理用データに変換
        memcpy_s(&data, sizeof(CNetDataAll), str, sizeof(CNetDataAll));

        //プレイヤーデータ更新
        for (int i = 0; i < MAX_PLAYER; i++) {
            PLAYER[i] = data.player[i];//変更
        }
        //敵情報更新
        for (int i = 0; i < MAX_ENEMY; i++) {
            ENEMY[i] = data.enemy[i];
        }
        for (int i = 0; i < MAX_ITEM; i++) {
            ITEM[i] = data.item[i];
        }
    }
}
