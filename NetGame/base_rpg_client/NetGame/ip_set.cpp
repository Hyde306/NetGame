#pragma once
#include "DxLib.h"

// char->int に変換(数値）
int ctoi(char c);

//IP設定
IPDATA IP_set() {
	IPDATA ip{ 0,0,0,0 };

	int numImg = LoadGraph("ipset_image\\number.png");
	int buttonImg = LoadGraph("ipset_image\\button.png");

	//IP数値の初期化と宣言
	//固定する場合は、ここにIPアドレスを初期値をして入力
	char str[12]{
		/*
		'1','7','2',//p1
		'0','1','7',//p2
		'0','9','0',//p3
		'1','0','0',//p4
		*/		
		'1','9','2',//p1
		'1','6','8',//p2
		'0','0','1',//p3
		'0','0','2',//p4
		/*
		'1','7','2',//p1
		'0','1','7',//p2
		'0','6','0',//p3
		'0','1','0',//p4
		*/
	};

	//マウスカーソル位置
	int MouseX{ 0 }, MouseY{ 0 };
	int button{ 0 };//マウスの左ボタン記憶用

	//マウスカーソル表示の有無
	//SetMouseDispFlag(TRUE);

	while (TRUE) {
		//escで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		ClearDrawScreen();

		//左クリック
		if (GetMouseInput() == MOUSE_INPUT_LEFT && !button) {
			//マウスの位置情報取得
			GetMousePoint(&MouseX, &MouseY);

			int k = 0;
			for (int j = 0; j < 512; j += 128) {
				for (int i = 0; i < 93; i += 32) {
					//クリックした位置のボタン判定
					//上ボタン
					if (MouseX > i + j && MouseX < i + j + 32 && MouseY > 16 && MouseY < 48) {
						str[k]++;
						if (str[k] >= '9' + 1)str[k] = '0';
					}
					//下ボタン
					if (MouseX > i + j && MouseX < i + j + 32 && MouseY > 80 && MouseY < 96) {
						str[k]--;
						if (str[k] <= '0' - 1)str[k] = '9';
					}
					k++;
				}
			}

			//OKボタン
			if (MouseX > 4 && MouseX < 53 && MouseY>128 && MouseY < 158) {
				int ipNo[4]{ 0,0,0,0 };
				char link[3]{ '0','0','0' };

				link[0] = str[0];
				link[1] = str[1];
				link[2] = str[2];
				ip.d1 = atoi(link);

				link[0] = str[3];
				link[1] = str[4];
				link[2] = str[5];
				ip.d2 = atoi(link);

				link[0] = str[6];
				link[1] = str[7];
				link[2] = str[8];
				ip.d3 = atoi(link);

				link[0] = str[9];
				link[1] = str[10];
				link[2] = str[11];
				ip.d4 = atoi(link);

				break;
			}
		}

		//描画
		DrawString(0, 0, "接続先のIP入力", GetColor(255, 255, 255));
		for (int i = 0, j = 0; i < 15; i++) {
			if ((i + 1) % 4 != 0) {
				DrawRectGraph(i * 32, 16, 0, 0, 32, 32, buttonImg, TRUE, 0, 0);//上側ボタン
				DrawRectGraph(i * 32, 80, 0, 0, 32, 32, buttonImg, TRUE, 0, 1);//下側ボタン
				DrawRectGraph(i * 32, 48, ctoi(str[j]) * 32, 0, 32, 32, numImg, TRUE, 0, 0);//数字
				j++;
			}
			else {
				//カンマ
				DrawRectGraph(i * 32 + 16, 80, 81, 0, 4, 4, buttonImg, TRUE, 0, 0);
			}
		}
		//OKボタン
		DrawRectGraph(4, 128, 32, 0, 49, 30, buttonImg, TRUE, 0, 0);

		//ボタンの情報を保存
		button = GetMouseInput();

		ScreenFlip();

		//escキーで処理中断
		if (CheckHitKey(KEY_INPUT_ESCAPE) || (ProcessMessage() == -1)) break;
	}
	//画面クリア
	ClearDrawScreen();

	return ip;
}

// char -> int
// 数値以外は'-1'を返す
int ctoi(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	return -1;
}