//ゲームシーン
#include "game.h"
#include "CollisionUpdate.h"
#include "function.h"
#include "player.h"
#include "octrock.h"

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
	ObjList add_list;

    timer++;
    //送信処理(3フレーム毎にデータ送信)
    if (timer >= 3) {
        client.Send(base);
        timer = 0;
    }//追加処理用オブジェクトリスト

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