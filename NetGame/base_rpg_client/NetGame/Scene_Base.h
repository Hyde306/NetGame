#pragma once
#include "Scene_Manager.h"
#include "objBase.h"

class CManager;//シーン管理クラス

//シーンベースクラス
class CScene {
protected:
	CManager* manager{ nullptr };
public:
	CScene(CManager* pManager) { manager = pManager; };

	//更新処理
	virtual int UpDate() { return 0; };
	//描画処理
	virtual void Draw() {};
	//オブジェクトのコピー
	virtual void ReceiveObj(ObjList&& obj) {};
	//デストラクタ
	virtual ~CScene() {};
};