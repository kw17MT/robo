#pragma once
#include "EnemyAtack.h"
#include "EnemyMove.h"
#include "EnemyRotation.h"

class EnemyBrain
{
private:
	Vector3 m_playerPos = Vector3::Zero;		//プレイヤーの位置
	Vector3 m_enemyPos = Vector3::Zero;			//敵（自分）の位置
	Quaternion m_rot = Quaternion::Identity;

	bool m_canShoot = false;
public:
	/**
	 * @brief エネミーの生成時に作成した移動速度を設定する。
	 * @param moveSpeed ランダムな移動速度
	*/
	void SetMoveSpeed(const float moveSpeed) { m_enemyMove.SetMoveSpeed(moveSpeed); }
	/**
	 * @brief プレイヤーの位置座標を記憶する
	 * @param playerPos プレイヤーの位置座標
	*/
	void MemoryPlayerPos(Vector3 playerPos) { m_playerPos = playerPos; }
	
	/**
	 * @brief 敵（自分）の位置座標を記憶する。
	 * @param enemyPos 敵（自分の位置座標
	*/
	void MemoryEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }

	/**
	 * @brief 主に計算済みの敵（自分）の位置を取得する
	 * @return 敵（自分）の位置
	*/
	Vector3 GetNextEnemyPos() { return m_enemyPos; }

	Quaternion GetEnemyRotation() { return m_rot; }

	/**
	 * @brief 移動モードを判断する。
	*/
	//void JudgeMoveType();

	bool JudgeCanShoot();

	/**
	 * @brief 頭を働かす。すべての判断を一気に行う
	*/
	void Work();
private:
	EnemyAtack m_enemyAtack;			//攻撃するかどうかの判断
	EnemyMove m_enemyMove;				//移動先を考える
	EnemyRotation m_enemyRotation;
};

