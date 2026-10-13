//ゲームシーンヘッダ
#pragma once
#include "main.h"
#include "Scene_Manager.h"
#include "objBase.h"
#include "CCamera.h"

#include "map.h"

class CGame :public CScene
{
private:
public:
	//オブジェクト
	ObjList base;

	//マップオブジェクト
	unique_ptr<CMap>map;

	//コンストラクタ
	CGame(CManager*);

	//デストラクタ
	~CGame();

	int UpDate();//更新処理
	void Draw();//描画処理

	//指定したオブジェクトを数える
	int CountObj(int _id) {
		int count = 0;
		for (auto& obj : base) {
			if (obj->ID == _id && obj->FLAG) count++;
		}
		return count;
	}
	//オブジェクトを引き継ぐ
	void ReceiveObj(ObjList obj) {
		for (auto& x : obj)
			base.push_back(move(x));
	}
	//CCamera* camera;//カメラオブジェクト
};