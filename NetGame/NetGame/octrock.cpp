
#include "octrock.h"
#include "function.h"
#include "damageObj.h"
#include "spawnPoint.h"
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

        if (spawn != nullptr)
            spawn->enemyAlive = false;

        return 0;
    }

    // ノックバック処理
    if (UpdateKnockBack())
    {
        // ノックバックによる移動にも壁判定を適用
        // ※UpdateKnockBack() が先に座標を変更する方式の場合、
        //   ここで元の位置を復元することはできない。
        return 0;
    }

    // プレイヤーの座標を取得
    Point playerPos = Get_Point(base, (int)ObjID::PLAYER);

    // プレイヤーへの方向を計算
    float dx = static_cast<float>(playerPos.x - pos.x);
    float dy = static_cast<float>(playerPos.y - pos.y);

    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 0.001f)
    {
        // 敵の追従速度
        const float followSpeed = 2.0f;

        // 移動ベクトルを正規化
        float moveX = dx / dist * followSpeed;
        float moveY = dy / dist * followSpeed;

        // X方向の壁判定
        Point nextPos = pos;
        nextPos.x += static_cast<int>(std::round(moveX));

        if (map->CanMove(nextPos, ImgWidth, ImgHeight))
            pos.x = nextPos.x;

        // Y方向の壁判定
        // X方向の移動結果を反映して判定
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
