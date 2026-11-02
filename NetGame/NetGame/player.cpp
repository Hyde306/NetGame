#include "player.h"
#include "function.h"
#include "sword.h"

CPlayer::CPlayer(CMap* _map)
{
	sprite.img = Resource::player_img;
	sprite.cutX = 0;
	sprite.cutY = 0;
	sprite.width = ImgWidth = 32;
	sprite.height = ImgHeight = 32;

	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2 - 32;

	//マップ情報
	map = _map;

	// HP初期化
	HP(10);

	ID = (int)ObjID::PLAYER;
	pri = 1;
}

int CPlayer::Action(const ObjList& base, ObjList& add_base)
{
	// HPが0になったら消滅
	if (hp <= 0)
	{
		hp = 0;
		FLAG = false;
		return 0;
	}

	// 無敵時間を減らす
	if (damageCoolTime > 0)
		damageCoolTime--;

	//攻撃中は操作を受け付けない
	if (att_frame > 0) {
		att_frame--;
		return 0;
	}

	vec.x = vec.y = 0;

	//ノックバック処理
	if (UpdateKnockBack()) return 0;

	//移動処理
	if (CheckHitKey(KEY_INPUT_A))vec.x = -1.0f;
	if (CheckHitKey(KEY_INPUT_D))vec.x = 1.0f;
	if (CheckHitKey(KEY_INPUT_W))vec.y = -1.0f;
	if (CheckHitKey(KEY_INPUT_S))vec.y = 1.0f;

	//移動しているのかフラグ
	bool isMove = (vec.x != 0 || vec.y != 0);

	//移動方向からキャラクターの向きを取得
	Dir = GetDir(Dir);

	//向きに応じて、切り取り位置を変更
	if (Dir == (int)DIR::DOWN) {
		//下向きの
		sprite.cutY = 32;
	}
	else if (Dir == (int)DIR::UP) {
		//上向き
		sprite.cutY = 0;
	}
	else {
		//左右方向
		sprite.LR_reverse_flag = true;
		//左の場合は、画像を反転させない。
		if (Dir >= (int)DIR::LEFT_DOWN && Dir <= (int)DIR::LEFT_UP)
			sprite.LR_reverse_flag = false;
		//左右方向の画像
		sprite.cutY = 64;
	}

	//攻撃処理
	if (Check_TriggerKey(KEY_INPUT_SPACE)) {
		add_base.push_back(make_unique<CSword>(pos, Dir, map));
		//攻撃後の硬直フレームを設定
		att_frame = 13;
	}

	if (isMove) {
		//移動ベクトル
		vec = Vector_SetLength(vec, 4.0f);
		//移動後の座標
		Point nextPos = pos;

		//x方向
		nextPos.x += vec.x;
		if (map->CanMove(nextPos, ImgWidth, ImgHeight))
			pos.x = nextPos.x;
		//y方向
		//現在の位置をもう一度コピー
		//x方向の移動結果を反映した位置からy方向を判定する
		nextPos = pos;
		nextPos.y += vec.y;
		if (map->CanMove(nextPos, ImgWidth, ImgHeight))
			pos.y = nextPos.y;
	}
	return 0;
}

// レベルアップ処理
void CPlayer::LevelUp()
{
	while (exp >= nextExp)
	{
		// 必要経験値を消費
		exp -= nextExp;

		// レベルアップ
		level++;

		// 次の必要経験値を増やす
		nextExp += 5;

		// 最大HPを2増やす
		maxhp += 2;

		// HP全回復
		hp = maxhp;
	}
}

void CPlayer::DrawHPBar()
{
	int barX = 20;
	int barY = 20;

	int barWidth = 200;
	int barHeight = 20;

	// HPバー
	float hpRate = 0.0f;

	if (maxhp > 0)
		hpRate = (float)hp / maxhp;

	if (hpRate < 0.0f) hpRate = 0.0f;
	if (hpRate > 1.0f) hpRate = 1.0f;

	// HP背景
	DrawBox(barX, barY,
		barX + barWidth, barY + barHeight,
		GetColor(0, 0, 0), TRUE);

	// HPゲージ
	DrawBox(barX, barY,
		barX + (int)(barWidth * hpRate),
		barY + barHeight,
		GetColor(0, 220, 0), TRUE);

	// HP枠
	DrawBox(barX, barY,
		barX + barWidth, barY + barHeight,
		GetColor(255, 255, 255), FALSE);

	DrawFormatString(
		barX, barY + 2,
		GetColor(255, 255, 255),
		"HP %d / %d", hp, maxhp
	);

	// EXPバー
	float expRate = 0.0f;

	if (nextExp > 0)
		expRate = (float)exp / nextExp;

	if (expRate < 0.0f) expRate = 0.0f;
	if (expRate > 1.0f) expRate = 1.0f;

	int expY = 55;

	// EXP背景
	DrawBox(barX, expY,
		barX + barWidth, expY + 14,
		GetColor(0, 0, 0), TRUE);

	// EXPゲージ
	DrawBox(barX, expY,
		barX + (int)(barWidth * expRate),
		expY + 14,
		GetColor(0, 150, 255), TRUE);

	// EXP枠
	DrawBox(barX, expY,
		barX + barWidth, expY + 14,
		GetColor(255, 255, 255), FALSE);

	// レベルと経験値
	DrawFormatString(
		barX, expY + 18,
		GetColor(255, 255, 255),
		"LEVEL %d   EXP %d / %d",
		level, exp, nextExp
	);
}

void CPlayer::Draw()
{
	Point draw_pos = CameraToScreen(pos, map->camera);

	sprite.Draw(draw_pos.x, draw_pos.y);

	// HPバー表示
	DrawHPBar();
}

