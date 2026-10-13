#include "client.h"
#include "player.h"
#include "library_function.h"
#include <cstring>

bool CClient::Connect(const IPDATA ip)
{
    //指定したIPアドレスとポートに接続
    netHandle = ConnectNetWork(ip, 40);//40...ポート番号
    if (netHandle != -1)
    {
        return true;
    }
    return false;
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
    }

    //構造体のデータを送信用バッファへコピー
    memcpy_s(str, sizeof(PlayerSendData), &data, sizeof(PlayerSendData));

    //サーバーへデータを送信
    NetWorkSend(netHandle, str, sizeof(PlayerSendData));
}
