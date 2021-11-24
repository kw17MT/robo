#pragma once
class SpriteRender;

class EnemyIconOnRader : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
public:
	bool Start();
	void Update();
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetRotation(Quaternion rot) { m_rot = rot; }
private:
	SpriteRender* m_spriteRender = nullptr;
};

