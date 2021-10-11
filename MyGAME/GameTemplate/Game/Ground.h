#pragma once
class SkinModelRender;

class Ground : public IGameObject
{
private:
public:
	bool Start();
private:
	SkinModelRender* m_skinModelRender = nullptr;
};

