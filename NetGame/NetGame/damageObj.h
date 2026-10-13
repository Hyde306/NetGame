#pragma once
#include "objBase.h"

//ダメージ処理をするオブジェクトクラス
class CDamageObj :public BaseVector{
public:
	//ノックバック移動ベクトル
	Vector knockVec{ 0,0 };
	//ノックバクフレーム
	int knockFrame{ 0 };

	void KnockBack(Vector);//ノックバック開始
	bool UpdateKnockBack();//ノックバック処理

	//体力
	int hp{ 0 };
	int maxhp{ 0 };

	void HP(int);//体力初期化

	int damageCoolTime{ 0 };//無敵時間
	//ダメージ処理(ダメージ量,ノックバックベクトル)
	void Damage(int, Vector);
};