#include "net_player.h"
#include "function.h"
#include "sword.h"

CNetPlayer::CNetPlayer(PlayerData* _data, CMap* _map) {
    sprite.img = Resource::player_img;
    sprite.cutX = 0;
    sprite.cutY = 0;

    sprite.width = ImgWidth = 32;
    sprite.height = ImgHeight = 32;

    map = _map;

    data = _data;

    ID = (int)ObjID::CNetPlayer;
    pri = 1;
}

int CNetPlayer::Action(const ObjList& base, ObjList& add_base)
{
    //接続が切れたら削除
    if (data->netState == NetState::LOST) {
        FLAG = false;
        return 0;
    }

    if (!data->alive) {
        FLAG = false;
        return 0;
    }

    data->exp = exp;

    //補間して移動させる。
    pos.x += (data->pos.x - pos.x) * 0.2f;
    pos.y += (data->pos.y - pos.y) * 0.2f;

    //向きに応じて、切り取り位置を変更
    if (data->dir == (int)DIR::DOWN) {
        //下向きの
        sprite.cutY = 32;
    }
    else if (data->dir == (int)DIR::UP) {
        //上向き
        sprite.cutY = 0;
    }
    else {
        //左右方向
        sprite.LR_reverse_flag = true;
        //左の場合は、画像を反転させない。
        if (data->dir >= (int)DIR::LEFT_DOWN && data->dir <= (int)DIR::LEFT_UP)
            sprite.LR_reverse_flag = false;
        //左右方向の画像
        sprite.cutY = 64;
    }

    //攻撃処理
    if (old_attackCount != data->attackCount) {
        //送信されたアタックカウントが異なる場合攻撃
        add_base.push_back(make_unique<CSword>(pos, data->dir, map));
        //アタックカウントを更新
        old_attackCount = data->attackCount;
    }

    return 0;
}

void CNetPlayer::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);

    DrawFormatString(
        draw_pos.x - 32, draw_pos.y - 32,
        0xffffff,
        "%d",
        exp
    );
}
