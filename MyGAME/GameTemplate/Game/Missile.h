#pragma once
class SkinModelRender;
class Enemy;

#include "Weapon.h"

enum EnMoveStage
{
	enDeploying,
	enStraightTarget,
	enChaseTarget
};

class Missile : public IGameObject
{
private:
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_deployDirection = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_moveDirection = g_camera3D->GetForward();
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_prevMoveDirection = Vector3::Zero;
	float count = 0;						//寿命
	bool m_firstCalc = false;				//最初の速度計算ができたか
	int m_number = 0;
public:
	Missile() {}
	~Missile();
	bool Start();
	void Update();

	/**
	 * @brief 発射地点から着弾予定地点のベクトルを求める
	 * @return 発射地点から着弾予定地点の単位ベクトル
	*/
	Vector3 CalcToTargetVec();

	void SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos) /*override;*/
	{
		m_position = pos;
		m_targetPos = targetPos;
	}

	void SetNumber(int number) { m_number = number; }

	void SetEnemy(Enemy* enemy) { m_enemy = enemy; }

	void RestrictRotation();

	//ミサイルの発射時のみランダムな方向に展開
	Vector3 CalcDeployDirection();
private:
	SkinModelRender* m_skinModelRender = nullptr;
	Enemy* m_enemy = nullptr;
	EnMoveStage m_moveStage = enDeploying;

	float a[5];
};

