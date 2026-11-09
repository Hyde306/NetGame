#include "item.h"
#include "player.h"

void CItem::GetItem(BaseVector* b) {
    CPlayer* p = dynamic_cast<CPlayer*>(b);

    if (p == nullptr) return;

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

    FLAG = false;
}