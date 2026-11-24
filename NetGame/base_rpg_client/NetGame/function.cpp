#include "function.h"

//描画用座標(オブジェクト座標,カメラ座標)
Point CameraToScreen(Point p, Point cp) {
	p.x = p.x - cp.x;
	p.y = p.y - cp.y;

	return p;
}

//NetPlayerの存在チェック
CNetPlayer* GetNetPlayer(const ObjList& base, int ID) {
	for (auto& obj : base)
	{
		//NetPlayer以外はスキップ
		if (obj->ID != (int)ObjID::CNetPlayer) continue;

		//NetPlayerにキャスト
		CNetPlayer* p = (CNetPlayer*)obj.get();

		//指定したIDのNetPlayerを発見
		if (p->netID == ID) return p;
	}

	//指定したIDのNetPlayerが存在しない
	return nullptr;
}