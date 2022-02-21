#pragma once
class SkinModelRender;
class Enemy;
class Effect;
class MissileBallistic;

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
	Vector3 m_position = Vector3::Zero;							//ミサイルの位置座標
	Vector3 m_deployDirection = Vector3::Zero;					//展開する方向
	Vector3 m_moveSpeed = Vector3::Zero;						//計算済みの移動速度
	Vector3 m_moveDirection = g_camera3D->GetForward();			//次移動する方向
	Quaternion m_rot = Quaternion::Identity;					//ミサイルの回転
	Vector3 m_prevMoveDirection = Vector3::Zero;				//1フレーム前の移動方向
	float m_lifeSpan = 0.0f;									//寿命
	bool m_firstCalc = false;									//最初の速度計算ができたか
	float m_currentMoveSpeedRate = 0.0f;						//最大速度の何％出しているか

public:
	/**
	 * @brief コンストラクタ
	*/
	Missile() {}

	/**
	* @brief デストラクタ
	*/
	~Missile();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 発射地点から着弾予定地点のベクトルを求める
	 * @return 発射地点から着弾予定地点の単位ベクトル
	*/
	Vector3 CalcToTargetVec();

	/**
	 * @brief 初期発射位置を設定する
	 * @param pos 
	*/
	void SetLaunchedPos(Vector3 pos) { m_position = pos; }

	/**
	 * @brief 自分がターゲットする敵のインスタンスを設定する
	 * @param enemy 
	*/
	void SetEnemy(Enemy* enemy) { m_enemy = enemy; }

	/**
	 * @brief 追尾の抑制を行う
	*/
	void RestrictRotation();

	//ミサイルの発射時のみランダムな方向に展開
	Vector3 CalcDeployDirection();
private:
	SkinModelRender* m_skinModelRender = nullptr;				//モデルインスタンス
	Enemy* m_enemy = nullptr;									//目標とする敵のインスタンス
	EnMoveStage m_moveStage = enDeploying;						//移動タイプ
	//MissileBallistic* m_ballistic = nullptr;					//弾道計算インスタンス
};

