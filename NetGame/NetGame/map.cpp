#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "map.h"
#include "function.h"
#include "spawnPoint.h"
#include "itemKey.h"

//文字分割関数
vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

//コンストラクタ
CMap::CMap() {
	//マップチップ画像
	img = Resource::map_img;

	//マップ配列初期化
	for (int y = 0; y < MAP_CHIP_Y; y++) {
		for (int x = 0; x < MAP_CHIP_X; x++) {
			map[y][x] = -1;
		}
	}

	//カメラ座標初期位置
	camera.x = 0;
	camera.y = 0;

	//マップデータ読み込み(後でメンバー関数化）
	int stage = 1;
	ifstream fp("image\\map.txt");
	string str;

	if (!fp.fail()) {
		while (getline(fp, str)) {
			if (str[0] == '@') {
				str.erase(str.begin());//@を削除
				int num = atoi(str.c_str());//int型に変換
				if (stage == num) {
					//ステージデータ読み込み
					for (int y = 0; y < MAP_CHIP_Y; y++) {
						getline(fp, str);
						vector<string> strv = split(str, ',');
						for (int x = 0; x < MAP_CHIP_X; x++) {
							map[y][x] = stoi(strv.at(x));
						}
					}
					break;
				}
			}
		}
		fp.close();
	}
}

//特殊ブロック生成
void CMap::Map_Create(ObjList& base) {
	for (int y = 0; y < MAP_CHIP_Y; y++) {
		for (int x = 0; x < MAP_CHIP_X; x++) {

			// 通常敵スポーンポイント
			if (map[y][x] == 30) {
				Point p{ x * CHIP_SIZE_X, y * CHIP_SIZE_Y };
				base.push_back(
					make_unique<CSpawnPoint>(p, 0, this)
				);
			}

			// ボス敵スポーンポイント
			else if (map[y][x] == 31) {
				Point p{ x * CHIP_SIZE_X, y * CHIP_SIZE_Y };
				base.push_back(
					make_unique<CSpawnPoint>(p, 1, this)
				);
			}

			// 鍵アイテム
			else if (map[y][x] == 32) {
				Point p{
					x * CHIP_SIZE_X + CHIP_SIZE_X / 2,
					y * CHIP_SIZE_Y + CHIP_SIZE_Y / 2
				};

				base.push_back(
					make_unique<CItemKey>(p, this)
				);

				// 鍵を生成した場所は床にする
				map[y][x] = 0;
			}
		}
	}
}

//マップ更新
void CMap::Map_Update(const ObjList& base, ObjList& add_base)
{
	//プレイヤーの位置情報保存
	Point p = Get_Point(base, (int)ObjID::PLAYER);

	//カメラ座標の計算
	float tx = p.x - DRAW_WIDTH / 2;
	float ty = p.y - DRAW_HEIGHT / 2;
	camera.x += (tx - camera.x) * 0.1f;
	camera.y += (ty - camera.y) * 0.1f;

	//カメラ領域判定
	if (camera.x <= CAMERA_INIT.x) camera.x = CAMERA_INIT.x;
	if (camera.y <= CAMERA_INIT.y) camera.y = CAMERA_INIT.y;
	if (camera.x >= CAMERA_END.x) camera.x = CAMERA_END.x;
	if (camera.y >= CAMERA_END.y) camera.y = CAMERA_END.y;

	//描画開始する配列の位置
	BaseChipNoX = (camera.x) / CHIP_SIZE_X;
	BaseChipNoY = (camera.y) / CHIP_SIZE_Y;

	//描画するチップ数（画面端の欠け防止のため2チップ分余裕を持たせる）
	ChipNumX = DRAW_WIDTH / CHIP_SIZE_X + 2;
	ChipNumY = DRAW_HEIGHT / CHIP_SIZE_Y + 2;
}

//移動可能かチェック(移動後の座標,判定横幅,判定立幅)
bool CMap::CanMove(Point p, int width, int height) {
	//座標を左上規準に修正
	p.x = p.x - width / 2;
	p.y = p.y - height / 2;

	//判定する４隅の座標
	Point ver[4]{
		{ p.x / CHIP_SIZE_X ,p.y / CHIP_SIZE_Y },
		{ (p.x + width - 1) / CHIP_SIZE_X,p.y / CHIP_SIZE_Y },
		{ (p.x + width - 1) / CHIP_SIZE_X,(p.y + height - 1) / CHIP_SIZE_Y },
		{ p.x / CHIP_SIZE_X ,(p.y + height - 1) / CHIP_SIZE_Y }
	};

	//四隅チェック
	for (int i = 0; i < 4; i++) {
		int x = (int)ver[i].x;
		int y = (int)ver[i].y;
		//配列範囲チェック
		if (x < 0 || x >= MAP_CHIP_X ||
			y < 0 || y >= MAP_CHIP_Y)
			return false;

		// マップチップ取得
		int chip = map[y][x];

		// 鍵を持っていて扉に接触した場合
		if (chip == 33 && keyObtained)
		{
			OpenDoor();
			chip = map[y][x];
		}

		// 壁判定
		if (IsWall(chip)) return false;
	}

	return true;
}

//指定したマップチップが壁かどうかを判定
bool CMap::IsWall(int chip)
{
	return (chip != 0 && chip != 9 && chip != 27 &&
		chip != 28 && chip != 18 &&
		chip != 30 && chip != 31 &&
		chip != 32 && chip != 34);
}

void CMap::OpenDoor()
{
	if (!keyObtained)
		return;

	for (int y = 0; y < MAP_CHIP_Y; y++)
	{
		for (int x = 0; x < MAP_CHIP_X; x++)
		{
			if (map[y][x] == 33)
			{
				map[y][x] = 34;
			}
		}
	}
}

//マップ描画
void CMap::Draw() {
	//マップチップを描画
	for (int y = 0; y < ChipNumY; y++) {
		for (int x = 0; x < ChipNumX; x++) {
			int mx = BaseChipNoX + x;
			int my = BaseChipNoY + y;

			int chip = map[my][mx];

			DrawRectGraph(
				(int)(mx * CHIP_SIZE_X - camera.x),
				(int)(my * CHIP_SIZE_Y - camera.y),
				(chip % 10) * CHIP_SIZE_X,
				(chip / 10) * CHIP_SIZE_Y,
				CHIP_SIZE_X,
				CHIP_SIZE_Y,
				img,
				true,
				false
			);
		}
	}
}