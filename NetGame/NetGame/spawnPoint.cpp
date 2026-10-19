
#include "spawnPoint.h"
#include "function.h"
#include "octrock.h"
#include "bossRock.h"
#include <cmath>

CSpawnPoint::CSpawnPoint(Point p, int no, CMap* _map)
{
    sprite.img = Resource::map_img;

    // 座標をオブジェクトの中心に修正
    pos.x = p.x + CHIP_SIZE_X / 2;
    pos.y = p.y + CHIP_SIZE_Y / 2;

    EnemyNo = no;
    map = _map;
}

int CSpawnPoint::Action(const ObjList& base, ObjList& add_base)
{
    // 出現した敵が存在する場合は何もしない
    if (enemyAlive)
        return 0;

    timer++;

    if (timer >= 30)
    {
        // プレイヤーの座標を取得
        Point p = Get_Point(base, (int)ObjID::PLAYER);

        float dx = static_cast<float>(p.x - pos.x);
        float dy = static_cast<float>(p.y - pos.y);
        float dist = std::sqrt(dx * dx + dy * dy);

        // プレイヤーが近づいたら敵を出現させる
        if (dist <= 150.0f)
        {
            switch (EnemyNo)
            {
            case 0:
                // 通常敵
                add_base.push_back(
                    std::make_unique<COctrock>(pos, map, this)
                );
                break;

            case 1:
                // 岩石の巨人ボス
                add_base.push_back(
                    std::make_unique<CBossRock>(pos, map, this)
                );
                break;

            default:
                // 未対応の番号では生成しない
                return 0;
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
