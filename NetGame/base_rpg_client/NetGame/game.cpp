//ゲームシーン
#include "game.h"
#include "CollisionUpdate.h"
#include "function.h"
#include "player.h"
#include "octrock.h"
#include "expitem.h"
#include <set>

IPDATA IP_set();//ip入力用の関数
IPDATA ip;//接続先のipアドレス

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){
	//リソースの読み込み
	Resource::Load();

	//マップオブジェクト
	map = make_unique<CMap>();

	//プレイヤー
	base.push_back(make_unique<CPlayer>(map.get()));

	//敵
	//Point pos{ 320,200 };
	//base.push_back(make_unique<COctrock>(pos,map.get()));

	//特殊ブロック生成
	map->Map_Create(base);

    //接続先のipアドレス設定
    ip = IP_set();

    //接続処理
    int time{ 5 };
    while (time>0) {
        //接続するまでループ
        //ネットワークハンドルの取得
        if (client.Connect(ip)) break;//接続処理
        ClearDrawScreen();
        time--;
        drawText(0, 0, 0xffffff, "接続中・・・%d", time, 30);
        ScreenFlip();
    }
}

//更新処理
int CGame::UpDate(){
	ObjList add_list;//追加処理用オブジェクトリスト

	client.Receive();

	CPlayer* player = dynamic_cast<CPlayer*>(Get_obj(base, (int)ObjID::PLAYER));
	if (player && client.myPlayerID != -1) {
		int exp = client.PLAYER[client.myPlayerID].exp;
		if (exp != old_exp) {
			player->exp += exp - old_exp;
			player->LevelUp();
			old_exp = exp;
		}
	}

	timer++;
	//送信処理(3フレーム毎にデータ送信)
	if (timer >= 3) {
		client.Send(base);
		timer = 0;
	}

	//マップ更新
	map->Map_Update(base,add_list);
	//更新
	for (auto& obj : base)	
		obj->Action(base,add_list);

	//接続プレイヤーの生成チェック
	for (int i = 0; i < MAX_PLAYER; i++) {
		//自分のIDはスキップ
		if (i == client.myPlayerID) continue;
		if (!client.PLAYER[i].alive) continue;
		//接続中のプレイヤーかチェック
		if (client.PLAYER[i].PlayerID != -1) {
			//すでにNetPlayerが生成されているかチェック
			if (GetNetPlayer(base, i) == nullptr) {
				//まだ生成されてない場合、NetPlayerを生成
				add_list.push_back(make_unique<CNetPlayer>(
					i,
					&client.PLAYER[i], map.get()));
			}
		}

	}

	//敵の生成チェック
	std::set<int> aliveEnemy;
	for (int i = 0; i < MAX_ENEMY; i++) {
		if (client.ENEMY[i].enemyID == -1) continue;
		aliveEnemy.insert(client.ENEMY[i].enemyID);

		//該当するIDの敵を検索
		auto enemy = FindEnemy(client.ENEMY[i].enemyID);

		if (!enemy){
			//該当オブジェクトなし
			//敵の生成
			add_list.push_back(make_unique<COctrock>(
				client.ENEMY[i].pos,
				map.get(),
				client.ENEMY[i].enemyID
			));
		}
		else {
			//該当オブジェクトあり
			//データ更新
			enemy->pos = client.ENEMY[i].pos;
		}
	}

	for (auto& obj : base) {
		auto enemy = dynamic_cast<CEnemyData*>(obj.get());
		if (!enemy) continue;
		if (aliveEnemy.find(enemy->enemyID) == aliveEnemy.end())
			obj->FLAG = false;
	}

	std::set<int> aliveItem;
	for (int i = 0; i < MAX_ITEM; i++) {
		if (client.ITEM[i].itemID == -1) continue;
		aliveItem.insert(client.ITEM[i].itemID);

		auto item = FindItem(client.ITEM[i].itemID);

		if (!item) {
			auto newItem = make_unique<CItemExp>(client.ITEM[i].pos, map.get());
			newItem->itemID = client.ITEM[i].itemID;
			add_list.push_back(move(newItem));
		}
		else {
			item->pos = client.ITEM[i].pos;
		}
	}

	for (auto& obj : base) {
		auto item = dynamic_cast<CItem*>(obj.get());
		if (!item) continue;
		if (item->itemNo != ItemNo::EXP) continue;
		if (aliveItem.find(item->itemID) == aliveItem.end())
			obj->FLAG = false;
	}

	//オブジェクト追加処理
	for (auto& obj : add_list)
		base.push_back(move(obj));

	//当たり判定処理
	CollisionUpDate(base);

	//削除処理
	erase_if(base, [](const auto& obj) {return !obj->FLAG; });

	//オブジェクトのソート処理(クイックソート)指定したインデックス間
	ObjSort_Quick(base, 0, base.size() - 1);

	return 0;
}

//指定したIDの敵オブジェクトの検索
CDamageObj* CGame::FindEnemy(int id)
{
	for (auto& obj : base) {
		auto enemy = dynamic_cast<CEnemyData*>(obj.get());
		if (!enemy)continue;
		//同じIDがある場合
		if (enemy->enemyID == id)
			return dynamic_cast<CDamageObj*>(obj.get());
	}

	return nullptr;
}

CItem* CGame::FindItem(int id)
{
	for (auto& obj : base) {
		auto item = dynamic_cast<CItem*>(obj.get());
		if (!item) continue;
		if (item->itemNo == ItemNo::EXP && item->itemID == id)
			return item;
	}

	return nullptr;
}

//描画処理
void CGame::Draw()
{
	map->Draw();

	for (auto& obj : base)obj->Draw();

	SetFontSize(16);
	//オブジェクト個数
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());
	//3D軸の描画
	DrawLine3D(VGet(0, 0, 0),VGet(0, 0, 100),0x0000ff);
	DrawLine3D(VGet(0, 0, 0),VGet(100, 0, 0),0xff0000);
	DrawLine3D(VGet(0, 0, 0),VGet(0, 100, 0),0x00ff00);
}

CGame::~CGame()
{
	Resource::Release();
}

//c++17 Releaseでerase_ifでエラーが出る場合こちらを利用するかコンパイルバージョンを変更
/*
base.erase(remove_if(base.begin(), base.end(), [](const unique_ptr<BaseVector>& base)
	{return !base->FLAG;}),
	base.end());
*/