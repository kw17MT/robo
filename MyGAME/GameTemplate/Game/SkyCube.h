#pragma once
class SkinModelRender;

class SkyCube : public IGameObject
{
private:
public:
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;
};