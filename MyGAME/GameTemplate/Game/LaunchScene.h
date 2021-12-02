#pragma once
class SkyCube;
class LaunchPad;
class SkinModelRender;
class PlayerEffect;
class Sun;

class LaunchScene : public IGameObject
{
private:
	Vector3 m_roboPos = { 0.0f,295.0f,5000.0f };
	Vector3 m_lampPos = m_roboPos;
	float m_delay = 0.0f;
	float m_angle = 0.0f;
public:
	LaunchScene() {}
	~LaunchScene();
	bool Start();
	void Update();
private:
	LaunchPad* m_launchPad = nullptr;
	SkyCube* m_sky = nullptr;						//スカイキューブマップインスタンス
	SkinModelRender* m_robo = nullptr;
	PlayerEffect* m_effect;
	Sun* m_sun = nullptr;
	SkinModelRender* m_lamp[8] = { nullptr };
};

