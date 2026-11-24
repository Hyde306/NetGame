#pragma once
#include "2D_function.h"
#include "3D_function.h"
#include "library_function.h"

#include "net_player.h"

//描画用座標(オブジェクト座標,カメラ座標)
Point CameraToScreen(Point, Point);

//NetPlayerの存在チェック
CNetPlayer* GetNetPlayer(const ObjList&, int);