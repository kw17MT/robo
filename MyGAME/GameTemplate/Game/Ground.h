#pragma once
class SkinModelRender;

class Ground : public IGameObject
{
private:
public:
	Ground() {};
	~Ground();
	bool Start();
private:
	SkinModelRender* m_skinModelRender = nullptr;
	//PhysicsStaticObject m_physics;
};