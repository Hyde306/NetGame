#include "expitem.h"
#include "function.h"
#include "map.h"
#include "player.h"

CItemExp::CItemExp(Point p, CMap* _map)
{
    //アイテム画像の設定
    sprite.img = Resource::item_img;
    sprite.cutX = 192;
    sprite.cutY = 0;
    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    pos = p;
    map = _map;

    //生成時にランダムな方向へ飛び出すように移動ベクトルを設定
    vec.x = Range_Random_Number(-8, 8);
    vec.y = Range_Random_Number(-8, 8);
    //取得時の効果量（経験値）
    value = 1;
    //アイテムの種類設定
    itemNo = ItemNo::EXP;
    //描画順を設定
    pri = 2;
}

int CItemExp::Action(const ObjList& base, ObjList& add_base)
{
    return 0;
}
void CItemExp::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);
    sprite.Draw(draw_pos.x, draw_pos.y);
}