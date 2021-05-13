#pragma once
#include "SkinModelRender.h"
class Arrow : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale;
public:
	bool Start();
	void Update();

	void Float();

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }

	SkinModelRender* m_skinModelRender;
};

