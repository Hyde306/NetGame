
#include "bossRock.h"
#include "function.h"
#include "spawnPoint.h"
#include <cmath>

CBossRock::CBossRock(Point p, CMap* _map, CSpawnPoint* _sp)
{
    sprite.img = Resource::octrock_img;
    sprite.cutX = 0;
    sprite.cutY = 0;

    // 通常敵の2倍の大きさ
    sprite.width = ImgWidth = 64;
    sprite.height = ImgHeight = 64;

    map = _map;
    pos = p;

    // HPを20に設定
    HP(20);

    spawn = _sp;
    ID = (int)ObjID::OCTROCK;
    pri = 2;
}

int CBossRock::Action(const ObjList& base, ObjList& add_base)
{
    if (damageCoolTime > 0)
        damageCoolTime--;

    if (hp <= 0)
    {
        FLAG = false;

        if (spawn != nullptr)
            spawn->enemyAlive = false;

        return 0;
    }

    if (UpdateKnockBack())
        return 0;

    Point playerPos = Get_Point(base, (int)ObjID::PLAYER);

    float dx = static_cast<float>(playerPos.x - pos.x);
    float dy = static_cast<float>(playerPos.y - pos.y);
    float dist = std::sqrt(dx * dx + dy * dy);

    // プレイヤーに近づく
    const float followSpeed = 1.0f;
    const float attackRange = 48.0f;

    if (dist > attackRange && dist > 0.001f)
    {
        float moveX = dx / dist * followSpeed;
        float moveY = dy / dist * followSpeed;

        Point nextPos = pos;
        nextPos.x += static_cast<int>(std::round(moveX));

        if (map->CanMove(nextPos, ImgWidth, ImgHeight))
            pos.x = nextPos.x;

        nextPos = pos;
        nextPos.y += static_cast<int>(std::round(moveY));

        if (map->CanMove(nextPos, ImgWidth, ImgHeight))
            pos.y = nextPos.y;
    }

    // 近距離で攻撃するためのタイマー
    if (attackTimer > 0)
        attackTimer--;

    if (dist <= attackRange && attackTimer <= 0)
    {
        // TODO:
        // プレイヤーにダメージを与える処理は、
        // プレイヤー側のダメージ処理に合わせて追加する。
        attackTimer = attackCoolTime;
    }

    return 0;
}

void CBossRock::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);
    sprite.Draw(draw_pos.x, draw_pos.y);
}
