#pragma once
class Missile;
class AmmoGauge;
class Enemy;

class MissileGenerator : public IGameObject
{
private:
	Vector3 m_launchPos = Vector3::Zero;
	bool m_isPrepareLaunch = false;
	int m_launchNum = 0;

	int m_remaining_missile = 20;
	bool m_deleteMissileIcon = false;
	int m_lockOnTargetNum = 0;
	bool m_canTargetMore = true;
public:
	MissileGenerator() {}
	~MissileGenerator();
	bool Start();
	void Update();
	void SetLaunchPos(Vector3 pos) { m_launchPos = pos; }
	bool GetDeleteMissileIcon() { return m_deleteMissileIcon; }

	void SaveTargetedEnemy(Enemy* enemy);
	bool CanTargetMore() { return m_canTargetMore; }
private:
	std::vector<Missile*> m_missiles = { nullptr };
	std::vector<Enemy*> m_enemy = { nullptr };
	AmmoGauge* m_ammoGauge = nullptr;
};