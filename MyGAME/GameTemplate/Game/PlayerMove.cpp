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
	//R1�������ƃ_�b�V������
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//�ȑO�̃_�b�V���̃X�s�[�h�������Čv�Z���Ȃ���
		m_dashSpeed = Vector3::Zero;
		//L�X�e�B�b�N��G���Ă��炸�A�_�b�V������ƑO���ɍs���悤�ɂ��邽��if���K�v
		//�|���Ă�����Ƀ_�b�V��
		if (abs(g_pad[0]->GetLStickXF()) || abs(g_pad[0]->GetLStickYF()))
		{
			m_dashSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED * SPEED_UP_RATE;
			m_dashSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED * SPEED_UP_RATE;;
		}
		//�X�e�B�b�N���͂Ȃ���ΑO���Ƀ_�b�V��
		else
		{
			m_dashSpeed += m_forwardDirection * MOVE_SPEED * SPEED_UP_RATE;
		}
		//Y�����Ă�΋}�㏸�AA�͋}���~
		m_dashSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED * SPEED_UP_RATE;
		m_dashSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED * SPEED_UP_RATE;

		//�ʏ�̈ړ��X�s�[�h�͉��Z���Ȃ�
		m_nextPos -= m_currentSpeed;
	}

	//�_�b�V���X�s�[�h�����Z
	m_nextPos += m_dashSpeed;
	//���t���[���Ɍ��������_�b�V�����x�����Z���������߁A���Z
	m_dashSpeed /= 1.05f;
}

const void PlayerMove::Move(const Vector3 currentPos)
{
	//�p�����[�^���X�V
	m_nextPos = currentPos;
	//��ʂ̑O��
	m_forwardDirection = g_camera3D->GetForward();
	//��ʂ̉E����
	m_sideDirection = g_camera3D->GetRight();
	//��ʂ̏����
	m_upDirection = Cross(m_forwardDirection, m_sideDirection);

	//�X�e�B�b�N���͂ő���
	m_currentSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED;
	m_currentSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED;
	m_currentSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED;
	m_currentSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED;

	//�p���x�ɐ�����݂���
	if (m_currentSpeed.x < -SPEED_LIMIT) { m_currentSpeed.x = -SPEED_LIMIT; }
	if (m_currentSpeed.y < -SPEED_LIMIT) { m_currentSpeed.y = -SPEED_LIMIT; }
	if (m_currentSpeed.z < -SPEED_LIMIT) { m_currentSpeed.z = -SPEED_LIMIT; }
	if (m_currentSpeed.x > SPEED_LIMIT) { m_currentSpeed.x = SPEED_LIMIT; }
	if (m_currentSpeed.y > SPEED_LIMIT) { m_currentSpeed.y = SPEED_LIMIT; }
	if (m_currentSpeed.z > SPEED_LIMIT) { m_currentSpeed.z = SPEED_LIMIT; }

	//�ʏ푬�x�����Z����
	m_nextPos += m_currentSpeed;
	m_currentSpeed /= 1.05f;
}

Vector3 PlayerMove::Execute(Vector3 currentPos)
{
	Move(currentPos);
	Dash();

	return m_nextPos;
}