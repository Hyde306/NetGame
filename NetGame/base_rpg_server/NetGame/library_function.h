#pragma once
#include "objBase.h"
#include "2D_function.h"
#include "3D_function.h"

//オブジェクトソート関数(オブジェクトリスト,ソート開始位置,ソート終了位置)
void ObjSort_Quick(vector<unique_ptr<BaseVector>>&, int, int);

//指定範囲の乱数を取得(min,max)
int Range_Random_Number(int, int);
float Range_Random_NumberF(float, float);

//全てのキーが、離されるまで待機
void WAIT_RELEASE_ALL_KEY();

//全てのキーが、離されるまで待機（タイプ指定 DX_CHECKINPUT_KEY : DX_CHECKINPUT_PAD : DX_CHECKINPUT_MOUS)
void WAIT_RELEASE_ALL_KEY(int);

//キーを押した瞬間だけtrueになる
int Check_TriggerKey(int);

//数値の描画（画像ハンドル, 一桁目の位置x, 一桁目の位置y, 値, 文字サイズw, 文字サイズh)
void DrawNum(int, int, int, int, int, int);

//リストから指定オブジェクトの取得
BaseVector* Get_obj(const ObjList&, int);

//指定したオブジェクトの座標取得
Point Get_Point(const ObjList&, int);

//指定したオブジェクト以外をリストから削除
void Del_SpecifiedObj(ObjList&, int);

//指定オブジェクトをリストから削除(FLAGをfalseにする)
void Del_ListObj(ObjList&, int);

//指定オブジェクトをリストから削除範囲(FLAGをfalseにする)
void Del_ListObj(ObjList&, int, int);

//描画関係--------------------------------------------------------------------------------

//影を付けた文字列を表示する関数(x,y,色,文字,数値,大きさ)
void drawText(int , int , int , const char* , int , int );