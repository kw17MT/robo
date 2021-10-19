#pragma once
class SkinModelRender;

class MachinGun : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
	float count = 0;
public:
	bool Start();
	void Update();
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetTargetPosition(Vector3 targetPos) { m_targetPos = targetPos; }
	Vector3 CalcToTargetVec();
private:
	SkinModelRender* m_skinModelRender = nullptr;
};

