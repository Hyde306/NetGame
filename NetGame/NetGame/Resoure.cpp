//ƒŠƒ[ƒX
#include "Resource.h"

//int Resource::img;
int Resource::map_img;
int Resource::player_img;
int Resource::octrock_img;
int Resource::item_img;
int Resource::bossrock_img;

//‰æ‘œ“Ç‚İ‚İ
void Resource::Load()
{
	//img = LoadGraph("");
	map_img = LoadGraph("image\\map.png");
	player_img = LoadGraph("image\\player32.png");
	octrock_img = LoadGraph("image\\octrock.png");
	item_img = LoadGraph("image\\item.png");
	bossrock_img = LoadGraph("image\\enemy6.png");
}

//‰æ‘œíœ
void Resource::Release() {
	//DeleteGraph(img);
	DeleteGraph(map_img);
	DeleteGraph(player_img);
	DeleteGraph(octrock_img);
	DeleteGraph(item_img);
	DeleteGraph(bossrock_img);
}

