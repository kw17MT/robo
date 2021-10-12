#include "stdafx.h"
#include "PlayerMove.h"

namespace 
{
const float MOVE_SPEED = 30.0f;
const float SPEED_UP_RATE = 100.0f;
const float SPEED_LIMIT = 5000.0f;
}

const void PlayerMove::Dash()
{
	//R1を押すとダッシュする
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//以前のダッシュのスピードを消して計算しなおし
		m_dashSpeed = Vector3::Zero;
		//Lスティックを触っておらず、ダッシュすると前方に行くようにするためif文必要
		//倒してる方向にダッシュ
		if (abs(g_pad[0]->GetLStickXF()) || abs(g_pad[0]->GetLStickYF()))
		{
			m_dashSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED * SPEED_UP_RATE;
			m_dashSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED * SPEED_UP_RATE;;
		}
		//スティック入力なければ前方にダッシュ
		else
		{
			m_dashSpeed += m_forwardDirection * MOVE_SPEED * SPEED_UP_RATE;
		}
		//Y押してれば急上昇、Aは急下降
		m_dashSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED * SPEED_UP_RATE;
		m_dashSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED * SPEED_UP_RATE;

		//通常の移動スピードは加算しない
		m_nextPos -= m_currentSpeed;
	}

	//ダッシュスピードを加算
	m_nextPos += m_dashSpeed;
	//次フレームに減速したダッシュ速度を加算したいため、除算
	m_dashSpeed /= 1.05f;
}

const void PlayerMove::Move(const Vector3 currentPos)
{
	//パラメータを更新
	m_nextPos = currentPos;
	//画面の前方
	m_forwardDirection = g_camera3D->GetForward();
	//画面の右方向
	m_sideDirection = g_camera3D->GetRight();
	//画面の上方向
	m_upDirection = Cross(m_forwardDirection, m_sideDirection);

	//スティック入力で操作
	m_currentSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED;
	m_currentSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED;
	m_currentSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED;
	m_currentSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED;

	//角速度に制限を設ける
	if (m_currentSpeed.x < -SPEED_LIMIT) { m_currentSpeed.x = -SPEED_LIMIT; }
	if (m_currentSpeed.y < -SPEED_LIMIT) { m_currentSpeed.y = -SPEED_LIMIT; }
	if (m_currentSpeed.z < -SPEED_LIMIT) { m_currentSpeed.z = -SPEED_LIMIT; }
	if (m_currentSpeed.x > SPEED_LIMIT) { m_currentSpeed.x = SPEED_LIMIT; }
	if (m_currentSpeed.y > SPEED_LIMIT) { m_currentSpeed.y = SPEED_LIMIT; }
	if (m_currentSpeed.z > SPEED_LIMIT) { m_currentSpeed.z = SPEED_LIMIT; }

	//通常速度を加算する
	m_nextPos += m_currentSpeed;
	m_currentSpeed /= 1.05f;
}

Vector3 PlayerMove::Execute(Vector3 currentPos)
{
	Move(currentPos);
	Dash();

	return m_nextPos;
}