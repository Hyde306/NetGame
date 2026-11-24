#include "server.h"
#include "net_player.h"
#include "player.h"
#include "library_function.h"
#include "item.h"

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
    for (int i = 1; i < MAX_PLAYER; i++) {
        //ネットワークハンドルが未使用なら、このスロットに登録
        if (PLAYER[i].netHandle == -1) {
            PLAYER[i].playerData = PlayerData{};
            //接続したクライアントのネットワークハンドルを保存
            PLAYER[i].netHandle = _netHandle;
            //接続状態を接続にする
            PLAYER[i].playerData.netState = NetState::CONNECTED;

            //接続したプレイヤーへIDを通知
            CJoinData join;
            join.PlayerID = i;
            //プレイヤーIDを設定
            PLAYER[i].playerData.PlayerID = i;
            //送信
            NetWorkSend(_netHandle, (char*)&join, sizeof(CJoinData));

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
    for (int i = 1; i < MAX_PLAYER; i++) {
        //受信データがあるか確認
        if (GetNetWorkDataLength(PLAYER[i].netHandle) > 0) {
            //通信データを受信
            NetWorkRecv(PLAYER[i].netHandle, (char*)&data, sizeof(PlayerSendData));
            //受信データを対応するプレイヤー情報へ反映
            PLAYER[i].playerData.pos = data.pos;//位置情報
            PLAYER[i].playerData.dir = data.dir;//向き
            PLAYER[i].playerData.attackCount = data.attackCount;//アタックカウント
            PLAYER[i].playerData.alive = data.alive;
        }
    }
}

//クライアントの切断チェック
void CServer::LostPlayer() {
    //切断したネットワークハンドル
    int handle = GetLostNetWork();

    if (handle == -1) return;

    //切断したプレイヤーの接続状態を変更
    for (int i = 1; i < MAX_PLAYER; i++) {
        if (PLAYER[i].netHandle == handle) {
            //切断状態にする
            PLAYER[i].playerData.netState = NetState::LOST;
            break;
        }
    }
}

//切断したプレイヤーのスロット開放処理
void CServer::ReleasePlayer() {
    for (int i = 1; i < MAX_PLAYER; i++) {
        if (PLAYER[i].playerData.netState == NetState::LOST)
        {
            //通信を終了
            CloseNetWork(PLAYER[i].netHandle);

            //スロットを空き状態にする
            PLAYER[i].netHandle = -1;
        }
    }

}

//サーバが管理している情報を接続中の全クライアントへ送信
void CServer::SendAll(const ObjList& base)
{
    //送信データ
    CNetDataAll data{ 0 };

    //サーバーのプレイヤーをプレイヤー情報PLAYER[0]に登録
    CPlayer* p = dynamic_cast<CPlayer*>(Get_obj(base, (int)ObjID::PLAYER));
    if (p) {
        PLAYER[0].playerData.PlayerID = 0;
        PLAYER[0].playerData.pos = p->pos;
        PLAYER[0].playerData.dir = p->Dir;
        PLAYER[0].playerData.attackCount = p->attackCount;
    }
    PLAYER[0].playerData.alive = (p != nullptr);

    //接続中のプレイヤー情報を送信データにコピー
    for (int i = 0; i < MAX_PLAYER; i++) {
        data.player[i] = PLAYER[i].playerData;
    }

    SetEnemyData(base, data);
    SetItemData(base, data);

    //接続している全プレイヤーへ送信
    for (int i = 0; i < MAX_PLAYER; i++) {
        //接続中の場合のみ送信
        if (PLAYER[i].netHandle != -1) {
            NetWorkSend(PLAYER[i].netHandle, (char*)&data, sizeof(CNetDataAll));
        }
    }
}

//敵情報収集
void CServer::SetEnemyData(const ObjList& base, CNetDataAll& data)
{
    int no = 0;

    aliveEnemy.clear();
    for (auto& obj : base) {
        CDamageObj* enemy = dynamic_cast<CDamageObj*>(obj.get());
        if (enemy && enemy->enemyID != -1) aliveEnemy.insert(enemy->enemyID);
    }

    for (auto& obj : base) {
        CDamageObj* enemy = dynamic_cast<CDamageObj*>(obj.get());
        if (!enemy) continue;
        if (enemy->ID == (int)ObjID::OCTROCK) {
            //管理用のenemyIDがまだ無ければ発行
            if (enemy->enemyID == -1) {
                enemy->enemyID = GetNextEnemyID();
                //発行したIDを使用中として登録
                aliveEnemy.insert(enemy->enemyID);
            }
            data.enemy[no].enemyID = enemy->enemyID;
            data.enemy[no].pos = enemy->pos;
            no++;
        }
    }
}

//管理用敵IDの発行
int CServer::GetNextEnemyID() {
    //0～MAX_ENEMY-1の中から未使用中のIDを探す
    for (int i = 0; i < MAX_ENEMY; i++) {
        //使用中のIDでなければ、IDを発行
        if (aliveEnemy.find(i) == aliveEnemy.end()) {
            return i;
        }
    }
    return -1;
}

void CServer::SetItemData(const ObjList& base, CNetDataAll& data)
{
    int no = 0;

    aliveItem.clear();
    for (auto& obj : base) {
        CItem* item = dynamic_cast<CItem*>(obj.get());
        if (item && item->itemID != -1) aliveItem.insert(item->itemID);
    }

    for (auto& obj : base) {
        CItem* item = dynamic_cast<CItem*>(obj.get());
        if (!item) continue;
        if (item->itemNo != ItemNo::EXP) continue;
        if (no >= MAX_ITEM) break;
        if (item->itemID == -1) {
            item->itemID = GetNextItemID();
            aliveItem.insert(item->itemID);
        }
        data.item[no].itemID = item->itemID;
        data.item[no].pos = item->pos;
        no++;
    }
}

int CServer::GetNextItemID() {
    for (int i = 0; i < MAX_ITEM; i++) {
        if (aliveItem.find(i) == aliveItem.end()) {
            return i;
        }
    }
    return -1;
}
