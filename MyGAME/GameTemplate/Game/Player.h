#pragma once
class SkinModelRender;
class MachinGun;
class MissileGenerator;
class Reticle;
class PlayerEN;
class RestrictArea;
class PlayerEffect;

#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"
#include "PlayerAnimation.h"
#include "PlayerHP.h"

#include "effect/Effect.h"

namespace
{
	const int animNum = 7;
}

enum EnDeathTypes
{
	enStillAlive,
	enByEnemy,
	enAwayFromArea
};

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = { 0.0f,400.0f,5000.0f };			//プレイヤーの現在の位置
	Vector3 m_prevHomePosition = Vector3::Zero;			//1フレーム前のホームポジションの位置
	Vector3 m_currentHomePosition = Vector3::Zero;				//カメラが追いかける位置座標

	EnDeathTypes m_deathType = enStillAlive;

	Quaternion m_effectRot = Quaternion::Identity;

	int a = 0;
public:
	/**
	 * @brief コンストラクタ
	*/
	Player() {};

	/**
	 * @brief デストラクタ
	*/
	~Player();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return 
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief プレイヤーの現在位置を取得する
	 * @return プレイヤーの現在位置
	*/
	Vector3 GetPosition() { return m_currentHomePosition; }

	void TakenDamage(EnPlayerDamageTypes damageType);

	Reticle* GetReticleInstance() { return m_reticle; }
private:
	SkinModelRender* m_skinModelRender = nullptr;		//プレイヤーのモデルインスタンス
	MachinGun* m_machingun = nullptr;					//プレイヤーが持つマシンガンインスタンス
	MissileGenerator* m_missileGene = nullptr;
	Reticle* m_reticle = nullptr;
	PlayerHP* m_playerHp = nullptr;
	PlayerEN* m_playerEn = nullptr;
	RestrictArea* m_area = nullptr;
	PlayerEffect* m_playerEffect = nullptr;

	CameraMove m_cameraMove;							//プレイヤーを追従するカメラの位置座標計算オブジェクト
	PlayerMove m_roboMove;								//プレイヤーの座標を計算する
	PlayerRotation m_roboRotation;						//プレイヤーの回転を計算する

	PlayerAnimation m_playerAnim;						//プレイヤーのアニメーション初期化オブジェクト
	AnimationClip m_animClip[animNum];					//プレイヤーのアニメーション
};

