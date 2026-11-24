#include "itemKey.h"
#include "function.h"
#include "map.h"
#include "player.h"

CItemKey::CItemKey(Point p, CMap* _map)
{
    // 鍵画像の設定
    sprite.img = Resource::key_img;
    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    // 座標とマップ
    pos = p;
    map = _map;

    // アイテムの種類
    itemNo = ItemNo::KEY;

    // 描画順
    pri = 2;
}

int CItemKey::Action(const ObjList& base, ObjList& add_base)
{
    for (const auto& obj : base)
    {
        if (obj->ID == (int)ObjID::PLAYER)
        {
            CPlayer* player = dynamic_cast<CPlayer*>(obj.get());

            if (player != nullptr)
            {
                float dx = player->pos.x - pos.x;
                float dy = player->pos.y - pos.y;

                float dist = sqrt(dx * dx + dy * dy);

                if (dist <= 32.0f)
                {
                    GetItem(player);
                    break;
                }
            }
        }
    }

    return 0;
}
void CItemKey::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);
    sprite.Draw(draw_pos.x, draw_pos.y);
}