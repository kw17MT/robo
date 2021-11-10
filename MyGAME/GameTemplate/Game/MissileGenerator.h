#pragma once
class Missile;
class AmmoGauge;

class MissileGenerator : public IGameObject
{
private:
	Vector3 m_launchPos = Vector3::Zero;
	bool m_isPrepareLaunch = false;
	bool m_isLaunch = false;
	int m_launchNum = 0;
	float m_lockOnInterval = 0.0f;

	int m_remaining_missile = 20;
	bool m_deleteMissileIcon = false;
public:
	MissileGenerator() {}
	~MissileGenerator();
	bool Start();
	void Update();
	void SetLaunchPos(Vector3 pos) { m_launchPos = pos; }
	bool GetDeleteMissileIcon() { return m_deleteMissileIcon; }
private:
	std::vector<Missile*> m_missiles = { nullptr };
	std::array<Vector3, 10> m_targets;
	AmmoGauge* m_ammoGauge = nullptr;
};