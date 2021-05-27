#pragma once
#include"SpriteRender.h"

class Fade;

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
	SpriteRender* m_spriteRender = nullptr;
	Fade* m_fade = nullptr;
};