#include "octrock.h"
#include "function.h"
#include "damageObj.h"
#include "spawnPoint.h"

COctrock::COctrock(Point p,CMap* _map, CSpawnPoint*_sp)
{
	sprite.img = Resource::octrock_img;
	sprite.cutX = 0;
	sprite.cutY = 0;
	sprite.width = ImgWidth = 32;
	sprite.height = ImgHeight = 32;

	//マップ情報
	map = _map;

	pos = p;

	HP(2);//HP初期化

	//スポーン位置保存
	spawn = _sp;

	ID = (int)ObjID::OCTROCK;
	pri = 2;
}

int COctrock::Action(const ObjList& base, ObjList& add_base)
{
	//無敵時間
	if (damageCoolTime > 0)damageCoolTime--;

	//HPがなくなった場合、オブジェクト削除
	if (hp <= 0) {
		FLAG = false;
		spawn->enemyAlive = false;
	}

	//ノックバック処理
	if (UpdateKnockBack())return 0;

	return 0;
}

void COctrock::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x, draw_pos.y);
}