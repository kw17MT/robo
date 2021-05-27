#pragma once
#include "SkinModelRender.h"
class Arrow : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	int m_timer = 0;
	float m_setYPos = 0.0f;
	bool m_scaleMaxFlag = false;
	bool m_scaleMinFlag = false;
public:
	Arrow();
	~Arrow();
	bool Start();
	void Update();

	void Float();
	void Expansion();
	void Shrink();

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	bool GetScaleMax() { return m_scaleMaxFlag; }
	bool GetScaleMin() { return m_scaleMinFlag; }

	SkinModelRender* m_skinModelRender = nullptr;
};

