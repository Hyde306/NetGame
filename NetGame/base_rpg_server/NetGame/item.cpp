#include "item.h"
#include "player.h"
#include "net_player.h"

void CItem::GetItem(BaseVector* b) {
    //取得した時の処理
    CPlayer* p = dynamic_cast<CPlayer*>(b);
    CNetPlayer* np = dynamic_cast<CNetPlayer*>(b);

    //プレイヤー
    if (p) {
        switch (itemNo) {
        case ItemNo::EXP:
            p->exp += value;
            break;

        case ItemNo::KEY:
            if (map != nullptr) {
                map->keyObtained = true;
            }
            break;
        }
    }

    //他のプレイヤー
    if (np) {
        switch (itemNo) {
        case ItemNo::EXP:
            np->exp += value;
            break;

        default:
            return;
        }
    }

    FLAG = false;
}