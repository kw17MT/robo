#pragma once
#include "SpriteRender.h"

/**
 * @brief �傫�Ȗ����̂Ȃ�UI��\������
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

