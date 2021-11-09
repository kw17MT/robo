#pragma once
class SkinModelRender;
class DisplayDistance;
class Player;
class EnemyStateIcon;
class EnemyBrain;
class MachinGun;

#include "EnemyHP.h"
#include "EnemyMove.h"
#include "EnemyBrain.h"

class Enemy : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//位置座標
	Quaternion m_rot = Quaternion::Identity;	//回転
	float m_distance = 0;						//敵とプレイヤーの距離		
public:
	/**
	 * @brief コンストラクタ
	*/
	Enemy() {};

	/**
	 * @brief デストラクタ
	*/
	~Enemy();

	/**
	 * @brief インスタンスが生成された時に一度だけ呼ばれる関数
	 * @return trueで正常終了
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 新しい位置座標を設定する。
	 * @param pos 新しい位置
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief この敵の位置座標を取得する
	 * @return 敵の位置座標
	*/
	Vector3 GetPosition() { return m_position; }

	/**
	 * @brief ダメージをHPに適用する
	 * @param damageType ダメージの種類
	*/
	void TakenDamage(EnDamageTypes damageType);
private:
	SkinModelRender* m_skinModelRender = nullptr;			//敵のモデル
	DisplayDistance* m_displayDistance = nullptr;			//プレイヤーと敵の距離を表示
	Player* m_player = nullptr;								//プレイヤーのインスタンス取得用
	EnemyStateIcon* m_enemyStateIcon = nullptr;				//敵にかかるアイコン全般を表示

	EnemyBrain m_enemyBrain;								//敵の行動をまとめて管理する
	EnemyHP* m_enemyHP = nullptr;							//敵のHPとHPバーを表示する
	//MachinGun* m_machinGun = nullptr;
};

