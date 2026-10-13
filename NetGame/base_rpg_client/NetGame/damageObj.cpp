#include "damageObj.h"
#include "map.h"

//ノックバック処理開始
void CDamageObj::KnockBack(Vector v) {
	//---------------------------------
	//ノックバックする方向と距離
	//---------------------------------
	knockVec = v;
	knockFrame = 10;

}

//ノックバック中の移動処理
bool CDamageObj::UpdateKnockBack() {
    if (knockFrame <= 0)
        return false;

    knockFrame--;

    //サブステップ判定
    //処理回数を計算
    int step = (int)max(abs(knockVec.x), abs(knockVec.y));
    step = max(step, 1);
    //1ステップの移動量を計算
    float moveX = knockVec.x / step;
    float moveY = knockVec.y / step;
    //ステップ判定
    for (int i = 0; i < step; i++) {
        //マップの判定
        Point nextPos = pos;
        nextPos.x += moveX;
        if (map->CanMove(nextPos, sprite.width, sprite.height))pos.x = nextPos.x;
        nextPos = pos;
        nextPos.y += moveY;
        if (map->CanMove(nextPos, sprite.width, sprite.height))pos.y = nextPos.y;
    }
    //ノックバックの勢いを徐々に減速
    knockVec.x *= 0.8f;
    knockVec.y *= 0.8f;

    return true;
}

//体力初期化
void CDamageObj::HP(int _hp) {
    hp = maxhp = _hp;
}

//ダメージ処理
void CDamageObj::Damage(int dm, Vector v) {
    if (damageCoolTime > 0) return;

    hp -= dm;
    KnockBack(v);
    damageCoolTime = 20;//20フレームはダメージ受けた後無敵
}