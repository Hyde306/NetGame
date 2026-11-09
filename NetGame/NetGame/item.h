#pragma once
#include "objBase.h"
#include "map.h"

class CItem : public BaseVector
{
public:
    CItem() {
        ID = (int)ObjID::ITEM;
    };

    void GetItem(BaseVector*);

    ItemNo itemNo{ -1 };
    int value{ -1 };

    CMap* map{ nullptr };
};