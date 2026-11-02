
#include "octrock.h"
#include "function.h"
#include "damageObj.h"
#include "spawnPoint.h"
#include "expitem.h"
#include <cmath>

COctrock::COctrock(Point p, CMap* _map, CSpawnPoint* _sp)
{
    sprite.img = Resource::octrock_img;
    sprite.cutX = 0;
    sprite.cutY = 0;
    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    // マップ情報
    map = _map;

    // 初期位置
    pos = p;

    // HP初期化
    HP(2);

    // スポーン位置保存
    spawn = _sp;

    ID = (int)ObjID::OCTROCK;
    pri = 2;
}

int COctrock::Action(const ObjList& base, ObjList& add_base)
{
    // 無敵時間
    if (damageCoolTime > 0)
        damageCoolTime--;

    // HPがなくなった場合、オブジェクト削除
    if (hp <= 0)
    {
        FLAG = false;

        // 経験値アイテムを生成
        add_base.push_back(
            std::make_unique<CItemExp>(pos, map)
        );

        if (spawn != nullptr)
            spawn->enemyAlive = false;

        return 0;
    }

    // ノックバック処理
    if (UpdateKnockBack())
        return 0;

    // プレイヤーの座標を取得
    Point playerPos = Get_Point(base, (int)ObjID::PLAYER);

    // プレイヤーとの距離を計算
    float dx = static_cast<float>(playerPos.x - pos.x);
    float dy = static_cast<float>(playerPos.y - pos.y);

    float dist = std::sqrt(dx * dx + dy * dy);

    // 探知範囲
    const float detectionRange = 200.0f;

    // 移動速度
    const float followSpeed = 2.0f;
    const float returnSpeed = 2.0f;

    // 移動先を決定
    Point targetPos;

    if (dist <= detectionRange)
    {
        // プレイヤーが探知範囲内なら追いかける
        targetPos = playerPos;
    }
    else
    {
        // 探知範囲外ならスポーン地点に戻る
        if (spawn == nullptr)
            return 0;

        targetPos = spawn->pos;
    }

    // 移動方向を計算
    float moveX = static_cast<float>(targetPos.x - pos.x);
    float moveY = static_cast<float>(targetPos.y - pos.y);

    float moveDist = std::sqrt(moveX * moveX + moveY * moveY);

    if (moveDist > 1.0f)
    {
        float speed = (dist <= detectionRange) ? followSpeed : returnSpeed;

        // 移動ベクトルを正規化
        moveX = moveX / moveDist * speed;
        moveY = moveY / moveDist * speed;

        // X方向の壁判定
        Point nextPos = pos;
        nextPos.x += static_cast<int>(std::round(moveX));

        if (map->CanMove(nextPos, ImgWidth, ImgHeight))
            pos.x = nextPos.x;

        // Y方向の壁判定
        nextPos = pos;
        nextPos.y += static_cast<int>(std::round(moveY));

        if (map->CanMove(nextPos, ImgWidth, ImgHeight))
            pos.y = nextPos.y;
    }

    return 0;
}
void COctrock::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);
}
