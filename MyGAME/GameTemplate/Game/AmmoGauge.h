#pragma once
class SpriteRender;
class FontRender;

enum EnAmmoTypes
{
	enAmmoBullet,
	enAmmoMissile,
	enAmmoRaser
};

class AmmoGauge : public IGameObject
{
private:
	Vector3 m_scale = { 2.0f,1.0f,1.0f };
	Vector3 m_screenPos = Vector3::Zero;
	Vector2 m_pivot = { 1.0f,0.0f };
	float m_fontScale = 0.5f;

	int m_remaining_ammo = 0;
	int m_max_ammo = 0;

	float m_reloadTimer = 0.0f;
	bool m_isReloaded = true;
	bool m_finishReloading = false;

	EnAmmoTypes m_ammoType = enAmmoBullet;
public:
	AmmoGauge() {}
	~AmmoGauge();

	bool Start();
	void Update();

	void Reloading(const float reloadTime);

	bool GetIsReloaded() { return m_isReloaded; }
	bool IsReloaded() { return m_finishReloading; }

	void SetAmmoType(EnAmmoTypes ammoType) { m_ammoType = ammoType; }
	void SetRemainingAmmo(int remainingAmmo) { m_remaining_ammo = remainingAmmo; }
	void SetMaxAmmo(int maxAmmo) { m_max_ammo = maxAmmo; }
	void SetPosition(Vector3 pos) { m_screenPos = pos; }
private:
	SpriteRender* m_spriteRender = nullptr;
	FontRender* m_fontRender = nullptr;
};

