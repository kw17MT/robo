#pragma once
#include "SpriteRender.h"

/**
 * @brief ‘å‚«‚È–ðŠ„‚Ì‚È‚¢UI‚ð•\Ž¦‚·‚é
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

