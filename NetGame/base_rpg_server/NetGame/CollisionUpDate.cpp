//当たり判定関数
#include "CollisionUpdate.h"
#include "function.h"
#include "player.h"
#include "damageObj.h"
#include "sword.h"
#include "expitem.h"

//当たり判定関数
//この関数を使用する場合、baseに登録しているオブジェクトの順番についてソートが必要
// 使用する場合の注意
//　①priの値を設定して、オブジェクトは、必ずソートしていること
//　②判定の条件を描く場合、aオブジェクトのpriが必ずbオブジェクトより小さくなるように指定すること
void Player_Enemy_Collsion(BaseVector* a, BaseVector* b) {
	//判定処理
	if (HitCheck_Box2(a, b)) {
		//当たった場合
		// //aをCDamageObjオブジェクトにキャスト
		CDamageObj* player = dynamic_cast<CDamageObj*>(a);
		//ノックバックする方向ベクトル
		//CPlayer* player = dynamic_cast<CPlayer*>(base[i].get());
		Vector v{
			a->pos.x - b->pos.x,
			a->pos.y - b->pos.y
		};
		//ノックバックする距離を設定
		v = Vector_SetLength(v, 15.0f);
		//ノックバック処理
		//player->KnockBack(v);
		player->knockVec = v;
		player->knockFrame = 10;
	}
}

//敵と剣
void Enemy_Sword_Collision(BaseVector* a, BaseVector* b) {

	//========================================
	// 敵を円、剣を線分として判定を行う
	//========================================

	//敵の当たり判定用半径
	float enemyRadius = 16;

	//========================================
	// 剣の始点と終点を求め、剣の線分を求める
	//========================================

	//剣オブジェクトを取得
	CSword* s = dynamic_cast<CSword*>(b);

	//剣の始点
	Point swordStart(s->pos.x, s->pos.y);

	//剣の角度と長さから終点を計算
	Point swordEnd{
		s->pos.x + cos(s->sprite.angle) * s->length,
		s->pos.y + sin(s->sprite.angle) * s->length
	};

	//========================================
	// 敵の中心から剣の線分までの最短距離を求める
	//========================================

	//敵の中心から、剣の線分上で最も近い点を取得
	Point p = Position_Closest_Line(a->pos, swordStart, swordEnd);

	//求めた点と敵の円判定
	float dx = a->pos.x - p.x;
	float dy = a->pos.y - p.y;
	float dist = sqrt(dx * dx + dy * dy);

	//========================================
	// 敵の中心から剣までの距離が半径以内なら当たり
	//========================================

	if (dist <= enemyRadius) {
		//敵にダメージ
		CDamageObj* enemy = dynamic_cast<CDamageObj*>(a);

		//ノックバックするベクトル(剣の最遠点の中心)
		Vector v{
			enemy->pos.x - s->pos.x,
			enemy->pos.y - s->pos.y
		};

		v = Vector_SetLength(v, 15);
		enemy->Damage(1, v);
	}
}

void Player_Item_Collision(BaseVector* a, BaseVector* b) {
	if (HitCheck_Box2(a, b)) {
		//アイテムオブジェクトにキャスト
		CItem* item = dynamic_cast<CItem*>(b);
		//アイテムゲット処理
		item->GetItem(a);
	}
}

void CollisionUpDate(ObjList& base) {
	for (int i = 0; i < base.size(); i++) {
		BaseVector* a = base[i].get();
		for (int j = i + 1; j < base.size(); j++)
		{
			BaseVector* b = base[j].get();

			if (a->ID > b->ID)swap(a, b);

			//プレイヤーと敵
			if (a->ID == (int)ObjID::PLAYER && b->ID == (int)ObjID::OCTROCK) {
				Player_Enemy_Collsion(a, b);
			}

			//剣と敵
			if (a->ID == (int)ObjID::OCTROCK && b->ID == (int)ObjID::SWORD) {
				Enemy_Sword_Collision(a, b);
			}

			//プレイヤーとアイテム
			if (a->ID == (int)ObjID::PLAYER && b->ID == (int)ObjID::ITEM) {
				Player_Item_Collision(a, b);
			}

			//ID順を統一
			//if(a==b)とif(b==a)は同じオブジェクトの判定になるので、ID順を統一する
		}
	}
}