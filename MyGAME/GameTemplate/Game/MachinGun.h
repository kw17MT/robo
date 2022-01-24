#pragma once
class Bullet;
class SkinModelRender;
class AmmoGauge;

#include "Weapon.h"

class MachinGun : public Weapon
{
private:
	Vector3 m_position = Vector3::Zero;			//マシンガンの位置
	Vector3 m_targetPos = Vector3::Zero;		//着弾させるターゲット座標
	Vector3 m_playerPos = Vector3::Zero;
	Vector3 m_armDirection = Vector3::Zero;
	Quaternion m_qRot = Quaternion::Identity;
	int m_remaining_bullet = 100;
	float m_shootDelay = 0.15f;
public:
	/**
	 * @brief コンストラクタ
	*/
	MachinGun() {};

	/**
	 * @brief デストラクタ
	*/
	~MachinGun();

	/**
	 * @brief オブジェクト生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 着弾地点と発射地点を設定する。
	 * @param targetPos 
	 * @param pos 
	*/
	void SetTargetPos(Vector3 targetPos) /*override;*/
	{
		m_targetPos = targetPos;
	}

	void SetPosition(Vector3 handPos, Vector3 playerPos, Vector3 armDirection)
	{
		m_position = handPos;
		m_playerPos = playerPos;
		m_armDirection = armDirection;
	}

private:
	SkinModelRender* m_skinModelRender = nullptr;	//マシンガンのモデル
	std::vector<Bullet*> m_bullets = { nullptr };	//発射される弾のインスタンス
	AmmoGauge* m_ammoGauge = nullptr;
};

