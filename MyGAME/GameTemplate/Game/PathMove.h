#pragma once
#include "PathFactory.h"

class CycleDirection;

enum enMoveState
{
	enNone,		//何もしない。
	enStart,	//動いてない。
	enMove,		//動いてる。
	enFinal		//最終ポイント到達。
};

enum enCycleDirection
{
	enNormal,
	enReverse,
};

class PlayerGene;
class Path;
class CSoundSourse;
struct Point;
class PathMove
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="move">移動速度。</param>
	/// <param name="enLane">レーンの種類。</param>
	/// <param name="guzaiNo">左か右か。</param>
	void Init(const Vector3& pos, const float move, EnLane enLane);

	void ChangeMoveSpeed(float move) { m_moveSpeed = move; }
	/// <summary>
	/// 移動させる。
	/// </summary>
	/// <returns>移動した後の座標。</returns>
	const Vector3& Move();

	EnLane GetLane()
	{
		return m_enLane;
	}

	//最終ポイントまでいったか？
	bool GetIsFinalPoint()
	{
		return m_enMoveState == enFinal;
	}

	enCycleDirection GetCycleDirection()
	{
		return m_cycleDirection;
	}

	//客に提供したバーガーの数が規定数に達したら、流れる向きを変える。
	void SwitchCycleDirection();

	PlayerGene* m_playerGene;

private:
	//回転方向スプライト
	CycleDirection* m_directSprite[3] = {nullptr};

	Path* m_path;											//パス。
	Point* m_point = nullptr;								//今向かってるポイント。
	Vector3 m_position;
	float m_moveSpeed;
	EnLane m_enLane = enNonLane;
	Vector3 	m_moveVector;		//移動ベクトル。
	enMoveState m_enMoveState = enStart;
	int m_guzaiNo = 0;
	enCycleDirection m_cycleDirection = enNormal;
	const int Num2ChangeCycle = 1;
	const int m_wayPointNum = 36;
};

