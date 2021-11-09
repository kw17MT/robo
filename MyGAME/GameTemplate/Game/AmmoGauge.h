#pragma once
class SpriteRender;

enum EnAmmoTypes
{
	enBullet,
	enMissile,
	enRaser
};

class AmmoGauge : public IGameObject
{
private:
	Vector3 m_scale = { 3.0f,1.0f,1.0f };
	Vector3 m_screenPos = Vector3::Zero;
	Vector2 m_pivot = { 1.0f,1.5f };

	int m_remaining_ammo = 0;
	int m_max_ammo = 0;

	float m_ReloadTimer = 0.0f;
	bool m_isReloaded = true;
	bool m_finisReloading = false;

	EnAmmoTypes m_ammoType = enBullet;
public:
	AmmoGauge() {}
	~AmmoGauge();

	bool Start();
	void Update();

	bool IsReloaded() { return m_finisReloading; }

	void SetAmmoType(EnAmmoTypes ammoType) { m_ammoType = ammoType; }
	void SetRemainingAmmo(int remainingAmmo) { m_remaining_ammo = remainingAmmo; }
	void SetMaxAmmo(int maxAmmo) { m_max_ammo = maxAmmo; }
private:
	SpriteRender* m_spriteRender = nullptr;
};

