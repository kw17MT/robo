#pragma once
class SkinModelRender;

class SkyCube : public IGameObject
{
private:
public:
	SkyCube() {}
	~SkyCube();
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;
};