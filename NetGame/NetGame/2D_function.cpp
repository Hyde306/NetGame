#pragma once
#include "2D_function.h"

//座標 + ベクトル
Point Add_Point_Vector(Point p0, Vector v0) {
	Point r;	
	r.x = p0.x + v0.x;	
	r.y = p0.y + v0.y;	
	return r; 
}
//座標 - 座標
Vector Sub_Point_Point(Point p0, Point p1) { Vector r;	r.x = p0.x - p1.x;	r.y = p0.y - p1.y;	return r; }
//座標 - 座標(x,y,x,y)
Vector Sub_Point_Point(float x1, float y1, float x2, float y2) {
	Vector r;	r.x = x1 - x2;	r.y = y1 - y2;	return r;}
//ベクトル - ベクトル
Vector Sub_Vector_Vector(Vector p0, Vector p1) { Vector r;	r.x = p0.x - p1.x;	r.y = p0.y - p1.y;	return r; }
//ベクトル + ベクトル
Vector Add_Vector_Vector(Vector v0, Vector v1) { 
	Vector r;	
	r.x = v0.x + v1.x;	
	r.y = v0.y + v1.y;	
	return r; 
}
//ベクトル × スカラー
Vector Mul_Vector_Scaler(Vector v, float s) { Vector r;	r.x = v.x * s;	r.y = v.y * s;	return r; }
//ベクトルの長さ
float Vector_Length(Vector v) { 
	float l = sqrtf((v.x * v.x + v.y * v.y));
	return l; 
}
//ベクトルの正規化
Vector Vector_Normalize(Vector v) { float l = Vector_Length(v);	Vector n;	n.x = v.x / l;	n.y = v.y / l;	return n; }
//ベクトルの長さを設定
Vector Vector_SetLength(Vector v, float length) { v = Vector_Normalize(v);	v = Mul_Vector_Scaler(v, length);	return v; }
//ベクトルの回転
Vector Vector_Rotation(Vector v, float angle) {	Vector r;	float l = Vector_Length(v);	v = Vector_Normalize(v);	r.x = v.x * cos(RADIAN(angle)) - v.y * sin(RADIAN(angle));	r.y = v.x * sin(RADIAN(angle)) + v.y * cos(RADIAN(angle));	return r;}
//内積
float Dot(Vector v1, Vector v2) {
	float r;
	v1 = Vector_Normalize(v1);
	v2 = Vector_Normalize(v2);

	r = v1.x * v2.x + v1.y * v2.y;

	return r;
}

//外積
float Cross(Vector v1, Vector v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

//指定した点から指定した直線上の最短点(点,直線の始点,直線の終点)
Point Position_Closest_Line(Point p, Point s, Point e) 
{
	Point P = p;//点

	Point A = s;//始点
	Point B = e;//終点
	Vector n{ B.x - A.x,B.y - A.y };//直線の方向ベクトル
	Point Q{ -1,-1 };//直線上の点

	Vector AP = Sub_Point_Point(P, A);//点から始点までのベクトル
	float t = (AP.x * n.x + AP.y * n.y) / (n.x * n.x + n.y * n.y);//投影係数

	if (t < 0)t = 0;//0より小さい場合は始点
	if (t > 1)t = 1.0f;//１より大きい場合は終点

	Q = Add_Point_Vector(A, Mul_Vector_Scaler(n, t));//直線上の点

	return Q;
}

//球判定(位置A,半径A,位置B,半径B)
bool HitCheck_Sphere(Point3 A, float RA, Point3 B, float RB) {
	if (DistanceF(A, B) < RA + RB)
		return true;
	return false;
}

//矩形判定(x1,y1,x2,y2,w,h)
bool HitCheck_Box(float x1, float y1, float x2, float y2, int w, int h) {
	if (x1 < x2 + w && x2 < x1 + w && y1 < y2 + h && y2 < y1 + h)
		return 1;
	return 0;
}

//矩形判定(x1,y1,x2,y2,w1,h1,w2,h2)
bool HitCheck_Box(float x1, float y1, float x2, float y2, int w1, int h1, int w2, int h2) {
	if (x1 < x2 + w2 && x2 < x1 + w1 &&	y1 < y2 + h2 && y2 < y1 + h1)
		return 1;
	return 0;
}

//矩形判定(オブジェクト,オブジェクト) 画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_Box(BaseVector* a, BaseVector* b) {
	if (a->pos.x < b->pos.x + b->ImgWidth &&
		b->pos.x < a->pos.x + a->ImgWidth &&
		a->pos.y < b->pos.y + b->ImgHeight &&
		b->pos.y < a->pos.y + a->ImgHeight)
		return true;
	return false;
}

//指定したIDと画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_Box(const ObjList& base, BaseVector* a, int ID) {
	for (int i = 0; i < base.size(); i++) {
		if (base[i]->ID == ID) {
			if (HitCheck_Box(a, base[i].get())) {
				return true;
			}
		}
	}
	return false;
}

//矩形判定(オブジェクト、オブジェクト）posが矩形の中心の場合
bool HitCheck_Box2(BaseVector* objA, BaseVector* objB) 
{
	RECT A{
		objA->pos.x - objA->ImgWidth / 2,
		objA->pos.y - objA->ImgHeight / 2,
		objA->pos.x + objA->ImgWidth / 2,
		objA->pos.y + objA->ImgHeight / 2
	};
	RECT B{
		objB->pos.x - objB->ImgWidth / 2,
		objB->pos.y - objB->ImgHeight / 2,
		objB->pos.x + objB->ImgWidth / 2,
		objB->pos.y + objB->ImgHeight / 2
	};

	if (
		A.left < B.right &&
		B.left < A.right &&
		B.bottom > A.top &&
		A.bottom > B.top
		)
	{
		return true;
	}

	return false;
}

//当たったオブジェクトを受け取る。指定したIDと画像サイズ(ImgWidth,ImgHeight)で判定
BaseVector* HitCheck_Box_GetObj(const ObjList& base, BaseVector* a, int ID) {
	for (int i = 0; i < base.size(); i++) {
		if (base[i]->ID == ID) {
			if (HitCheck_Box(a, base[i].get())) {
				return base[i].get();
			}
		}
	}
	return a;
}

//円と矩形判定用関数（矩形の一番近い位置を計算）
float Circle_Center_With_Closest_Point(float a, float b, float c)
{
	if (a < b) {
		return b;
	}
	else if (a > c) {
		return c;
	}
	else {
		return a;
	}
}
//円と矩形判定(円オブジェクト、矩形オブジェクト、円の半径）
bool HitCheck_Box_Circle(BaseVector* a, BaseVector* b, float radius) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x, b->pos.x + b->ImgWidth);
	n.y = Circle_Center_With_Closest_Point(pc.y, b->pos.y, b->pos.y + b->ImgHeight);

	float l = DistanceF(pc.x, pc.y, n.x, n.y);//一番近い座標から中心までの距離
	if (l < radius)//半径radiusで判定
	{
		return true;
	}
	return false;
}

//円と矩形判定(円オブジェクト、矩形オブジェクト、円の半径）めり込み修正
void HitCheck_Box_Circle2(BaseVector* a, BaseVector* b, float radius) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x, b->pos.x + b->ImgWidth);
	n.y = Circle_Center_With_Closest_Point(pc.y, b->pos.y, b->pos.y + b->ImgHeight);

	float l = DistanceF(pc.x, pc.y, n.x, n.y);//一番近い座標から中心までの距離
	if (l < radius)//半径radiusで判定
	{
		Vector v{ n.x - pc.x, n.y - pc.y };//方向ベクトル
		v = Vector_Normalize(v);//方向ベクトル正規化
		v = Vector_SetLength(v, radius - l);
		a->vec.x -= (int)v.x;
		a->vec.y -= (int)v.y;
	}
}

//二点間距離
float DistanceF(float a, float b, float c, float d)
{
	return sqrtf((a - c) * (a - c) + (b - d) * (b - d));
}
float DistanceF(Point3 A, Point3 B) {
	float r;
	r = sqrtf((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y) + (A.z - B.z) * (A.z - B.z));
	return r;
}

//指定した点と点の衝突
bool HitCheck_Point_Point(Point A, Point B) { 

	if (A.x == B.x && A.y == B.y) return true;
	return false; 
}

//指定した点と線分の衝突(点,始点,終点)
bool HitCheck_Point_Line(Point A, Point P1, Point P2) { return false; }

//点と矩形（回転あり）の衝突
bool HitCheck_Point_Box(Point P, BaseVector& obj) {
	//外積を使って計算する。
	//外積が、＋の場合半時計周り、－の場合時計回り
	//つまり、全ての辺と点のベクトルの外積を求めて
	//全てが－の場合、その矩形の内側に存在する点となる

	float angle = RADIAN(obj.angle);
	//回転矩形の元軸ベクトル
	Vector axis[2] =
	{
		{cosf(angle),sinf(angle)},
		{-sinf(angle),cosf(angle)}
	};
	//計算用各軸のベクトル
	Vector a = axis[0];
	Vector b = axis[1];

	//各頂点
	//左上,右上,右下,左下
	Vector v[4] = 
	{
		{obj.pos.x - a.x * obj.ImgWidth/2 + b.x * obj.ImgHeight/2, obj.pos.y - a.y * obj.ImgWidth/2 + b.y * obj.ImgHeight/2},
		{obj.pos.x + a.x * obj.ImgWidth/2 + b.x * obj.ImgHeight/2, obj.pos.y + a.y * obj.ImgWidth/2 + b.y * obj.ImgHeight/2},
		{obj.pos.x + a.x * obj.ImgWidth/2 - b.x * obj.ImgHeight/2, obj.pos.y + a.y * obj.ImgWidth/2 - b.y * obj.ImgHeight/2},
		{obj.pos.x - a.x * obj.ImgWidth/2 - b.x * obj.ImgHeight/2, obj.pos.y - a.y * obj.ImgWidth/2 - b.y * obj.ImgHeight/2}
	};
	
	//各辺と点の外積を計算
	for (int i = 0; i < 4; i++) 
	{
		Vector edge{
			v[(i + 1) % 4].x - v[i].x,
			v[(i + 1) % 4].y - v[i].y 
		};
		Vector toP{ P.x - v[i].x,P.y - v[i].y };


		if (Cross(edge, toP) > 0) return false;
	}
	return true; 
}

//点と三角形の衝突(点,頂点A,頂点B,頂点C)
bool HitCheck_Point_Triangle(Point P, Point A, Point B, Point C) { 

	//各頂点
	Vector v[3] =
	{
		{A.x, A.y},
		{B.x, B.y},
		{C.x, C.y}
	};

	//各辺と点の外積を計算
	for (int i = 0; i < 3; i++)
	{
		Vector edge{
			v[(i + 1) % 3].x - v[i].x,
			v[(i + 1) % 3].y - v[i].y
		};
		Vector toP{ P.x - v[i].x,P.y - v[i].y };

		if (Cross(edge, toP) > 0) return false;
	}
	return true;
}

//点と楕円の衝突
bool HitCheck_Point_Oval(Point A, BaseVector* obj) { return false; }

/*
//参考関数
//回転軸を求めたり、回転させたり
Vector Rotate(const Vector& v, float rad) {
	return {
		v.x * cosf(RADIAN(rad)) - v.y * sinf(RADIAN(rad)),
		v.x * sinf(RADIAN(rad)) + v.y * cosf(RADIAN(rad))
	};
}
void GetAxis(const BaseVector& b, Vector axis[2])
{
	axis[0] = Rotate({ 1,0 }, b.angle);
	axis[1] = Rotate({ 0,1 }, b.angle);
}
*/