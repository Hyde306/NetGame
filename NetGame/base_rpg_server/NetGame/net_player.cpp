#include "net_player.h"
#include "function.h"

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
    //•âŠÔ‚µ‚ÄˆÚ“®‚³‚¹‚éB
    pos.x += (data->pos.x - pos.x) * 0.2f;
    pos.y += (data->pos.y - pos.y) * 0.2f;

    return 0;
}

void CNetPlayer::Draw()
{
    Point draw_pos = CameraToScreen(pos, map->camera);

    sprite.Draw(draw_pos.x, draw_pos.y);
}
