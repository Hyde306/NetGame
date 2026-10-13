#include "server.h"
#include "net_player.h"

//接続処理
bool CServer::Accept(int port)
{
    //接続処理
    PreparationListenNetWork(port);
    NetHandle = GetNewAcceptNetWork();

    if (NetHandle == -1)
        return false;

    return true;
}

//サーバー側のプレイヤー情報へ登録する
void CServer::New_Receive(ObjList& add_base, int _netHandle, CMap* _map)
{
    //[0]はサーバー側プレイヤー専用
    //空いているスロットを探す
    for (int i = 1; i < 4; i++) {
        //ネットワークハンドルが未使用なら、このスロットに登録
        if (PLAYER[i].netHandle == -1) {
            //接続したクライアントのネットワークハンドルを保存
            PLAYER[i].netHandle = _netHandle;
            // 新しく接続したクライアント用のプレイヤーオブジェクトを生成
            add_base.push_back(make_unique<CNetPlayer>(&PLAYER[i].playerData, _map));
            break;
        }
    }
}

// データ受信
void CServer::Receive()
{
    //クライアントから受信する通信データ
    PlayerSendData data{};

    //接続中のプレイヤーを確認
    //[0]はサーバー側プレイヤー専用
    for (int i = 1; i < 4; i++) {
        //受信データがあるか確認
        if (GetNetWorkDataLength(PLAYER[i].netHandle) > 0) {
            //通信データを受信
            NetWorkRecv(PLAYER[i].netHandle, (char*)&data, sizeof(PlayerSendData));
            //受信データを対応するプレイヤー情報へ反映
            PLAYER[i].playerData.pos = data.pos;
        }
    }
}
