#pragma once
#include "item.h"
#include "map.h"

class CItemKey : public CItem
{
public:
    CItemKey(Point p, CMap* _map);

    int Action(const ObjList& base, ObjList& add_base);
    void Draw();
};