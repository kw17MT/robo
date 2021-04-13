#pragma once
#include "PathFactory.h"

enum enMoveState
{
	enNone,
	enStart,	//�����ĂȂ��B
	enMove,		//�����Ă��Ȃ��B
	enFinal		//�ŏI�|�C���g���B�B
};

class Path;
struct Point;
class PathMove
{
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="pos">���W�B</param>
	/// <param name="move">�ړ����x�B</param>
	/// <param name="enLane">���[���̎�ށB</param>
	/// <param name="guzaiNo">�����E���B</param>
	void Init(const Vector3& pos, const float move, EnLane enLane, const int guzaiNo);

	/// <summary>
	/// �ړ�������B
	/// </summary>
	/// <returns>�ړ�������̍��W�B</returns>
	const Vector3& Move();

	EnLane GetLane()
	{
		return m_enLane;
	}

	//�ŏI�|�C���g�܂ł��������H
	bool GetIsFinalPoint()
	{
		return m_enMoveState == enFinal;
	}
private:
	Path* m_path;											//�p�X�B
	Point* m_point = nullptr;								//���������Ă�|�C���g�B
	Vector3 m_position;
	float m_moveSpeed;
	EnLane m_enLane = enNonLane;
	Vector3 	m_moveVector;		//�ړ��x�N�g���B
	enMoveState m_enMoveState = enStart;
	int m_guzaiNo = 0;
};
