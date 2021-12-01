#pragma once
#include "SpriteRender.h"

enum EnReticleType
{
	enNormalReticle,
	enTargetReticle
};

class Reticle : public IGameObject
{
private:
	//スクリーン画面のホームポジション
	const Vector3 m_homePosition = { 0.5f,0.5f,0.0f };
	//敵をロックオンした時のポジション
	Vector3 m_lockOnPosition = Vector3::Zero;
	//レティクルの拡大率（1が通常レティクル、2がターゲット
	Vector3 m_reticleScale[2] = { Vector3::Zero, Vector3::Zero };
	//ターゲットしている敵の位置
	Vector3 m_targetingEnemyPos = Vector3::Zero;

	bool m_isTarget = false;				//現在ターゲットしているか
	//bool m_isDecidedNextTarget = false;		//次にターゲットする敵は決まっているか（敵側が知っている）
	float m_PressButtonTime = 0.5f;			//LB2ボタンを押している秒数
	float m_targetReticleRotateRate = 0.0f;
public:
	/**
	 * @brief コンストラクタ
	*/
	Reticle() {};

	/**
	 * @brief デストラクタ 
	*/
	~Reticle();

	/**
	 * @brief オブジェクト生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();


	void CalcPosition();

	/**
	 * @brief LB2ボタンの押し加減（どれだけ押したか）でターゲットの種類を識別する
	*/
	void ChangeTargetStateDependOnButtonLB2();

	/**
	 * @brief 現在ターゲットしているか設定する
	 * @param state 現在ターゲットしているか
	*/
	void SetIsTargeting(bool state) { m_isTarget = state; }

	//void SetIsDecidedNextTarget(bool isDecided) { m_isDecidedNextTarget = isDecided; }

	void SetTargetedEnemyPos(Vector3 pos) { m_targetingEnemyPos = pos; }

	/**
	 * @brief 現在ターゲットしているか
	 * @return TRUEしている　FALSEしてない
	*/
	bool GetIsTargeted() { return m_isTarget; }

	//bool GetIsDecidedNextTarget() { return m_isDecidedNextTarget; }

	Vector3 GetTargetingEnemyPos() { return m_targetingEnemyPos; }

private:
	std::array< SpriteRender*, 2> m_spriteRender = { nullptr, nullptr };
};