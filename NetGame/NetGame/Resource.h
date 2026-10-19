//リソースクラス
#pragma once
#include "main.h"

class Resource {
public:
	//static int img;

	static int map_img;
	static int player_img;
	static int octrock_img;
	static int item_img;
	static int bossrock_img;

	//リソースの読み込み
	static void Load();
	//各リソースの削除処理
	static void Release();
};

