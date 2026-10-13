//spawnPoint.cpp
#include "spawnPoint.h"
#include "function.h"
#include "octrock.h"

CSpawnPoint::CSpawnPoint(Point p, int no, CMap* _map) {
    sprite.img = Resource::map_img;

    //座標をオブジェクトの中心に修正
    pos.x = p.x + CHIP_SIZE_X / 2;
    pos.y = p.y + CHIP_SIZE_Y / 2;

    EnemyNo = no;
    map = _map;
}

int CSpawnPoint::Action(const ObjList& base, ObjList& add_base)
{
    //出現した敵が存在する場合何もしない
    if (enemyAlive) return 0;

    timer++;
    if (timer >= 30) {
        //プレイヤーの座標を取得
        Point p = Get_Point(base, (int)ObjID::PLAYER);
        float dx = p.x - pos.x;
        float dy = p.y - pos.y;
        float dist = sqrt(dx * dx + dy * dy);
        //敵出現
        if (dist <= 150) {
            //敵生成
            switch (EnemyNo) {
            case 0:
                add_base.push_back(make_unique<COctrock>(pos, map, this));
                break;
            }
            enemyAlive = true;
            timer = 0;
        }
    }

    return 0;
}

void CSpawnPoint::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x + vec.x, draw_pos.y + vec.y);
}