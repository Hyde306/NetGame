#pragma once

//オブジェクトID
enum class ObjID {
	PLAYER,
	CNetPlayer,
	OCTROCK,
	SWORD,
	ITEM,
	BOSSROCK,
};

//アイテムID
enum class ItemNo {
	EXP,
	KEY
};

//ネットワークステータス
enum class NetState {
	CONNECTED,	//接続中
	LOST,		//未接続
};