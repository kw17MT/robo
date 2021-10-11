#pragma once
#include "SpriteRender.h"
#include "Reticle.h"

class UI : public IGameObject
{
public:
	bool Start();
	void Update();
private:
	Reticle* m_reticle = nullptr;
};

