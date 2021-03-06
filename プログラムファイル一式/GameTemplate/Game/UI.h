#pragma once
#include "SpriteRender.h"

/**
 * @brief 大きな役割のないUIを表示する
*/
class UI : public IGameObject
{
public:
	UI() {};
	~UI();
	bool Start();
	void Update();
private:
	SpriteRender* m_sprite[4] = { nullptr };
};

