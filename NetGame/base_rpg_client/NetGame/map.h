#pragma once
#include "objBase.h"

class CMap {
public:
	//マップチップ数
	static constexpr int MAP_CHIP_X = 256;//256
	static constexpr int MAP_CHIP_Y = 88;//88
	//マップサイズ
	static constexpr int MAP_SIZE_X = CHIP_SIZE_X * MAP_CHIP_X;
	static constexpr int MAP_SIZE_Y = CHIP_SIZE_Y * MAP_CHIP_Y;
	//描画チップ数
	static constexpr int DRAW_CHIP_X = 14;
	static constexpr int DRAW_CHIP_Y = 12;
	//描画サイズ
	static constexpr int DRAW_WIDTH = DRAW_CHIP_X * CHIP_SIZE_X;
	static constexpr int DRAW_HEIGHT = DRAW_CHIP_Y * CHIP_SIZE_Y;
	//カメラの限界位置
	static constexpr Point CAMERA_INIT{ 0, 0 };
	static constexpr Point CAMERA_END{ MAP_CHIP_X * CHIP_SIZE_X - DRAW_WIDTH,MAP_CHIP_Y * CHIP_SIZE_Y - DRAW_HEIGHT };

	CMap();
	~CMap() { DeleteGraph(img); }

	int img{ -1 };
	//マップデータ
	int map[MAP_CHIP_Y][MAP_CHIP_X]{ 0 };

	//マップ更新処理
	void Map_Update(const ObjList&, ObjList&);
	void Map_Create(ObjList&);//特殊ブロック生成
	void Draw();

	//移動可能かチェック(位置情報, width, height)
	bool CanMove(Point, int, int);
	//指定したマップチップが壁かどうかを判定
	bool IsWall(int);

	//スクリーン座標
	Point screen_pos{ 0,0 };
	//カメラ座標
	Point camera{ 0,0 };

	//更新処理
	int BaseChipNoX{ 0 };//描画する左上のチップ番号(配列の添え字）
	int BaseChipNoY{ 0 };//描画する左上のチップ番号(配列の添え字）
	int ChipNumX{ 0 };//描画するチップ数X
	int ChipNumY{ 0 };//描画するチップ数Y
};