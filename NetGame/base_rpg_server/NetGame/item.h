#pragma once
#include "objBase.h"

class CItem :public BaseVector
{
public:
	CItem() {
		//当たり判定用のオブジェクトID設定
		ID = (int)ObjID::ITEM;
	};

	//アイテムを取得した時の処理
	void GetItem(BaseVector*);

	ItemNo itemNo{ -1 };

	int value{ -1 };//アイテム関係変数(回復量など)

};