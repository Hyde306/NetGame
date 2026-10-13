//ゲームシーン
#include "game.h"
#include "CollisionUpdate.h"
#include "function.h"
#include "player.h"
#include "octrock.h"

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
}

//更新処理
int CGame::UpDate(){
	ObjList add_list;

    //データ受信チェック
    server.Receive();

    //接続処理
    if (server.Accept(40))
    {
        //新しい接続処理
        server.New_Receive(add_list, server.NetHandle, map.get());
    }
//追加処理用オブジェクトリスト

	//マップ更新
	map->Map_Update(base,add_list);
	//更新
	for (auto& obj : base)	
		obj->Action(base,add_list);

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

    IPDATA IP;
    //起動端末のIPアドレス取得
    GetMyIPAddress(&IP, 1, NULL);
    DrawFormatString(0, WINDOW_HEIGHT - 20, 0xffffff,
        "この端末のIPアドレス %d:%d:%d:%d",
        IP.d1, IP.d2, IP.d3, IP.d4);
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