#pragma once
#include "objBase.h"

//座標 + ベクトル
Point Add_Point_Vector(Point, Vector);
//座標 - 座標
Vector Sub_Point_Point(Point, Point);
//座標 - 座標(x,y,x,y)
Vector Sub_Point_Point(float, float, float, float);
//ベクトル - ベクトル
Vector Sub_Vector_Vector(Vector, Vector);
//ベクトル + ベクトル
Vector Add_Vector_Vector(Vector, Vector);
//ベクトル × スカラー
Vector Mul_Vector_Scaler(Vector, float);
//ベクトルの長さ
float Vector_Length(Vector);
//ベクトルの正規化
Vector Vector_Normalize(Vector);
//ベクトルの長さを設定
Vector Vector_SetLength(Vector, float);
//ベクトルの回転
Vector Vector_Rotation(Vector, float);
//内積
float Dot(Vector, Vector);
//外積
float Cross(Vector, Vector);

//指定した点から見た指定した直線上の最短点(点,直線の始点,直線の終点)
Point Position_Closest_Line(Point, Point, Point);

//球判定(位置A,半径A,位置B,半径B)
bool HitCheck_Sphere(Point3, float, Point3, float);

//矩形判定(x1,y1,x2,y2,w,h)
bool HitCheck_Box(float, float, float, float, int, int);

//矩形判定(x1,y1,x2,y2,w1,h1,w2,h2)
bool HitCheck_Box(float, float, float, float, int, int, int, int);

//矩形判定(オブジェクト,オブジェクト)ImgWidth,ImgHeightで判定
bool HitCheck_Box(BaseVector*, BaseVector*);

//矩形判定(オブジェクト、オブジェクト）posが矩形の中心の場合
bool HitCheck_Box2(BaseVector*, BaseVector*);

//指定したIDと画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_Box(const ObjList&, BaseVector*, int);

//当たったオブジェクトを受け取る。指定したIDと画像サイズ(ImgWidth,ImgHeight)で判定
BaseVector* HitCheck_Box_GetObj(const ObjList&, BaseVector*, int);

//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
bool HitCheck_Box_Circle(BaseVector*, BaseVector*, float);

//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)円オブジェクトのベクトルを更新
void HitCheck_Box_Circle2(BaseVector*, BaseVector*, float);

//二点間距離
float DistanceF(float, float, float, float);
float DistanceF(Point3, Point3);

//指定した点と点の衝突
bool HitCheck_Point_Point(Point, Point);

//指定した点と線分の衝突(点,始点,終点)
bool HitCheck_Point_Line(Point, Point, Point);

//点と矩形（回転あり）の衝突
bool HitCheck_Point_Box(Point, BaseVector&);

//点と三角形の衝突(点,頂点A,頂点B,頂点C)
bool HitCheck_Point_Triangle(Point, Point, Point, Point);

//点と楕円の衝突
bool HitCheck_Point_Oval(Point, BaseVector*);