#pragma once
class SkinModelRender;
#include "SkinModelRender.h"

class Floor : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;

	bool isRed = false;
	Light m_lig;
public:
	void SetPosition(Vector3 pos) { m_position = pos; }

	~Floor();
	bool Start();
	void Update();

	void ChangeModel(const char* filePath) { m_skinModelRender->ChangeModel(filePath); }
	void SetRed(bool red) { isRed = red; }
	SkinModelRender* m_skinModelRender;
};

