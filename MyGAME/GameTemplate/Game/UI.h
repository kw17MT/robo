#pragma once
#include "SpriteRender.h"
#include "Reticle.h"

class UI : public IGameObject
{
public:
	UI() {};
	~UI();
	bool Start();
	void Update();
private:
	Reticle* m_reticle = nullptr;
};

