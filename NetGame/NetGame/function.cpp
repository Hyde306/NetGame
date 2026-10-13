#include "function.h"

//描画用座標(オブジェクト座標,カメラ座標)
Point CameraToScreen(Point p, Point cp) {
	p.x = p.x - cp.x;
	p.y = p.y - cp.y;

	return p;
}