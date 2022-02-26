#pragma once
class SpriteRender;

class EliminateTelop : public IGameObject
{
private:
	Vector3 m_position = { 2000.0f,150.0f, 0.0f };
	Vector3 m_scale = Vector3::One;
	float m_elapsedTime = 0.0f;
	bool m_shouldSlideInFrame = false;
	bool m_shouldDeleteTelop = false;
	bool m_shouldDestroyInstance = false;
public:
	EliminateTelop() {}
	~EliminateTelop();
	bool Start();
	void Update();

	const bool GetShouldDestroyInstance() const { return m_shouldDestroyInstance; }
private:
	SpriteRender* m_sprite[2] = { nullptr };
};

