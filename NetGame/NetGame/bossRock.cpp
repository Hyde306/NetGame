
#include "bossRock.h"
#include "function.h"
#include "spawnPoint.h"
#include <cmath>

CBossRock::CBossRock(Point p, CMap* _map, CSpawnPoint* _sp)
{
    sprite.img = Resource::bossrock_img;

    sprite.cutX = 0;
    sprite.cutY = 0;

    // 画像は1コマ32×32
    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    sprite.scale = 2.0;

    map = _map;
    pos = p;

    HP(20);

    spawn = _sp;

    ID = (int)ObjID::BOSSROCK;
    pri = 3;
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

    // 移動量を初期化
    vec.x = 0;
    vec.y = 0;

    if (dist > attackRange && dist > 0.001f)
    {
        vec.x = dx / dist * followSpeed;
        vec.y = dy / dist * followSpeed;

        // X方向の壁判定
        Point nextPos = pos;
        nextPos.x += static_cast<int>(std::round(vec.x));

        if (!map->CanMove(nextPos, ImgWidth, ImgHeight))
            vec.x = 0;

        // Y方向の壁判定
        nextPos = pos;
        nextPos.y += static_cast<int>(std::round(vec.y));

        if (!map->CanMove(nextPos, ImgWidth, ImgHeight))
            vec.y = 0;
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


    //移動方向からキャラクターの向きを取得
    Dir = GetDir(Dir);

    //向きに応じて、切り取り位置を変更
    if (Dir == (int)DIR::DOWN) {
        //下向きの
        sprite.cutY = 32;
    }
    else if (Dir == (int)DIR::UP) {
        //上向き
        sprite.cutY = 0;
    }
    else {
        //左右方向
        sprite.LR_reverse_flag = false;
        //左の場合は、画像を反転させない。
        if (Dir >= (int)DIR::LEFT_DOWN && Dir <= (int)DIR::LEFT_UP)
            sprite.LR_reverse_flag = true;
        //左右方向の画像
        sprite.cutY = 64;
    }

    pos.x += vec.x;
	pos.y += vec.y;
    
    return 0;
}

void CBossRock::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);
    sprite.Draw(draw_pos.x, draw_pos.y);
}
