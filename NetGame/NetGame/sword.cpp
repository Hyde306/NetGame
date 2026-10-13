#include "sword.h"
#include "function.h"

CSword::CSword(Point p, int _dir,CMap* _map)
{
	sprite.img = Resource::player_img;
	sprite.cutX = 96;
	sprite.cutY = 32;
	sprite.width = 32;
	sprite.height = 32;

	pos = p;
	Dir = _dir;

	map = _map;

	//剣の向き
	if (Dir == (int)DIR::UP) {
		angle = start_angle = 180;//上
	}
	else if (Dir == (int)DIR::DOWN) {
		angle = start_angle = 0;//下
	}
	else if (Dir >= (int)DIR::LEFT_DOWN && Dir <= (int)DIR::LEFT_UP) {
		angle = start_angle = -270;//左
	}
	else {
		angle = start_angle = 270;//右
	}

	//移動ベクトル
	vec.x = cos(RADIAN(angle)) * sprite.width;
	vec.y = sin(RADIAN(angle)) * sprite.height;
	sprite.angle = RADIAN(angle);

	pri = 2;
	ID = (int)ObjID::SWORD;
}

int CSword::Action(const ObjList& base, ObjList& add_base)
{
	angle += 15;
	vec.x = cos(RADIAN(angle)) * sprite.width;
	vec.y = sin(RADIAN(angle)) * sprite.height;

	sprite.angle = RADIAN(angle);

	//半周したら消す
	if (angle > start_angle + 180)FLAG = false;

	return 0;
}

void CSword::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x + vec.x, draw_pos.y + vec.y);
}