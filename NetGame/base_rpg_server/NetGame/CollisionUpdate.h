//当たり判定用関数
#pragma once
#include "objBase.h"

//当たり判定メイン関数
void CollisionUpDate(ObjList&);

//プレイヤーと敵
void Player_Enemy_Collision(BaseVector*, BaseVector*);

//敵と剣
void Enemy_Sword_Collision(BaseVector*, BaseVector*);

//プレイヤーとアイテム
void Player_Item_Collision(BaseVector*, BaseVector*);