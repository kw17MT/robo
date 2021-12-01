#pragma once
class SkinModelRender;
class Enemy;
class Effect;

#include "Weapon.h"

enum EnMoveStage
{
	enDeploying,
	enChaseTarget,
	enPassingBy
};

class Missile : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_deployDirection = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_moveDirection = g_camera3D->GetForward();
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_prevMoveDirection = Vector3::Zero;
	float count = 0;						//寿命
	bool m_firstCalc = false;				//最初の速度計算ができたか

	float m_currentMoveSpeedRate = 0.0f;

	bool m_isTargetEnemyDead = false;
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

	void SetLaunchedPos(Vector3 pos) { m_position = pos; }

	void SetEnemy(Enemy* enemy) { m_enemy = enemy; }

	void RestrictRotation();

	//ミサイルの発射時のみランダムな方向に展開
	Vector3 CalcDeployDirection();
private:
	SkinModelRender* m_skinModelRender = nullptr;
	Enemy* m_enemy = nullptr;
	EnMoveStage m_moveStage = enDeploying;
	//Effect* m_effect = nullptr;
};

