#pragma once
#include"SpriteRender.h"

class Title : public IGameObject
{
public:
	Title() {}
	~Title();
	//初期化処理。
	bool Start();
	//更新処理。
	void Update();
private:
	SpriteRender* m_spriteRender;

};

