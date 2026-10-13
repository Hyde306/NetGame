#pragma once
#include "main.h"
#include "objChar.h"

using namespace std;
class BaseVector;
class CMap;
using ObjList = vector<unique_ptr<BaseVector>>;

//スプライトクラス
//簡易描画クラス
class Sprite {
public:
	//描画元画像
	int img{ -1 };
	//画像サイズ
	int width{ 0 };
	int height{ 0 };
	//切り取り開始位置
	int cutX{ 0 };
	int cutY{ 0 };
	//回転中心位置(切り出した矩形内の座標）
	Point c{ (float)width/2,(float)height/2 };
	//大きさ
	double scale{ 1.0 };
	double scaleX{ 1.0 };
	double scaleY{ 1.0 };
	//角度
	double angle{ 0.0 };//ラジアン角
	float angleOffset{ 0.0f };//画像が上向きの場合"DX_PI_F/2"を設定
	//反転フラグ
	bool LR_reverse_flag{ false };
	bool UD_reverse_flag{ false };

	void Draw(float x, float y) const
	{
		DrawRectRotaGraph(
			x, y,
			cutX, cutY,
			width, height,
			scale,
			angle,
			img,
			true,
			LR_reverse_flag,
			UD_reverse_flag
		);
	}	
	void Draw2(float x, float y) const
	{
		DrawRectRotaGraph2(
			x,y,
			cutX,cutY,
			width,height,
			c.x,c.y,
			scale,
			angle,
			img,
			LR_reverse_flag,
			UD_reverse_flag
		);
	}
	void Draw3(float x, float y) const {
		DrawRectRotaGraph3(
			x, y,
			cutX, cutY,
			width, height,
			c.x, c.y,
			scaleX,scaleY,
			angle,
			img,
			LR_reverse_flag,
			UD_reverse_flag
		);
	}
};

//オブジェクトベースクラス(vector)
class BaseVector : public Character {
public:
	int pri{ 0 };//描画の順番
	int ID{ -1 };//オブジェクトID
	bool FLAG{ true };//オブジェクト存在フラグ
	bool draw_flag{ true };//オブジェクトの描画フラグ
	Sprite sprite;//簡易描画クラス

	//マップクラス
	CMap* map{ nullptr };

	//処理用のタイマー
	int timer{ 0 };

	//アニメーション処理用
	int animTimer{ 0 };
	int animFrame{ 0 };

	//オブジェクトリスト,追加オブジェクト用リスト
	virtual int Action(const ObjList&, ObjList&) = 0;
	virtual void Draw() = 0;
	virtual ~BaseVector() = default;

	//サイズの取得
	int GetWidth() const { return sprite.width * sprite.scale; }
	int GetHeight() const { return sprite.height * sprite.scale; }

	//向きを求める
	int GetDir(int old_dir) const {
		if (vec.x == 0 && vec.y > 0) return (int)DIR::DOWN;
		if (vec.x < 0 && vec.y > 0) return (int)DIR::LEFT_DOWN;
		if (vec.x < 0 && vec.y == 0) return (int)DIR::LEFT;
		if (vec.x < 0 && vec.y < 0) return (int)DIR::LEFT_UP;
		if (vec.x == 0 && vec.y < 0) return (int)DIR::UP;
		if (vec.x > 0 && vec.y < 0) return (int)DIR::RIGHT_UP;
		if (vec.x > 0 && vec.y == 0) return (int)DIR::RIGHT;
		if (vec.x > 0 && vec.y > 0) return (int)DIR::RIGHT_DOWN;
		return old_dir;
	}

	//位置取得
	Point GetPos()const { return pos; };

	//パッドの入力状況取得
	int PAD1() { return GetJoypadInputState(DX_INPUT_PAD1); }
	int PAD2() { return GetJoypadInputState(DX_INPUT_PAD2); }
	int PAD3() { return GetJoypadInputState(DX_INPUT_PAD3); }
	int PAD4() { return GetJoypadInputState(DX_INPUT_PAD4); }
};

//オブジェクトのソートクラス
class sort
{
public:
	bool operator()(const unique_ptr<BaseVector>& x, const unique_ptr<BaseVector>& y) const
	{
		return x.get()->pri < y.get()->pri;
	}
};

//オブジェクトのソートクラス（y座標でソート)
class sort_position_Y
{
public:
	bool operator()(const unique_ptr<BaseVector>& a, const unique_ptr<BaseVector>& b) const
	{
		return a.get()->pos.y < b.get()->pos.y;
	}
};


//3D用
class BaseVector3D : public Character3D {
public:
	int ID{ -1 };//オブジェクトID
	bool FLAG{ true };//オブジェクト存在フラグ
	bool draw_flag{ true };//オブジェクトの描画フラグ

	virtual int Action(vector<unique_ptr<BaseVector3D>>&) = 0;
	virtual void Draw() = 0;
};