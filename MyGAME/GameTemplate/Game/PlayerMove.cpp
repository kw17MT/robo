#include "stdafx.h"
#include "PlayerMove.h"

namespace 
{
const float MOVE_SPEED = 3.0f;
const float SPEED_UP_RATE = 100.0f;
const float SPEED_LIMIT = 1000.0f;
}

const void PlayerMove::Dash()
{
	//R1�������ƃ_�b�V������
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//�_�b�V�����Ă����Ԃɂ���
		m_isDash = true;
		
		//�ȑO�̃_�b�V���̃X�s�[�h�������Čv�Z���Ȃ���
		m_dashSpeed = Vector3::Zero;
		//L�X�e�B�b�N��G���Ă��炸�A�_�b�V������ƑO���ɍs���悤�ɂ��邽��if���K�v
		//�|���Ă�����Ƀ_�b�V��
		if (abs(g_pad[0]->GetLStickXF()) || abs(g_pad[0]->GetLStickYF()))
		{
			//���ɓ|���Ă��镪�����J�������猩�čēx�ɉ���
			m_dashSpeed += m_sideDirection * g_pad[0]->GetLStickXF() * MOVE_SPEED * SPEED_UP_RATE;
			//�c�ɓ|���Ă��镪�����J�������猩�ĉ��A��O�ɉ���
			m_dashSpeed += m_forwardDirection * g_pad[0]->GetLStickYF() * MOVE_SPEED * SPEED_UP_RATE;;
		}
		//�X�e�B�b�N���͂Ȃ���ΑO�������Ƀ_�b�V��
		else
		{
			m_dashSpeed += m_forwardDirection * MOVE_SPEED * SPEED_UP_RATE;
		}
		//L�X�e�B�b�N�ɉ���Y�����Ă�΋}�㏸�AA�͋}���~
		m_dashSpeed += m_upDirection * g_pad[0]->IsPress(enButtonY) * MOVE_SPEED * SPEED_UP_RATE;
		m_dashSpeed -= m_upDirection * g_pad[0]->IsPress(enButtonA) * MOVE_SPEED * SPEED_UP_RATE;

		//�ʏ�̈ړ��X�s�[�h�͉��Z���Ȃ�
		m_nextPos -= m_currentSpeed;
	}

	//�_�b�V���X�s�[�h�����Z
	m_nextPos += m_dashSpeed;
	//���t���[���Ƀ_�b�V���{�^���������Ă��Ȃ��Ă����������_�b�V�����x�����Z���������߁A���Z
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

	//�e���x�ɐ�����݂���
	if (m_currentSpeed.x < -SPEED_LIMIT) { m_currentSpeed.x = -SPEED_LIMIT; }
	if (m_currentSpeed.y < -SPEED_LIMIT) { m_currentSpeed.y = -SPEED_LIMIT; }
	if (m_currentSpeed.z < -SPEED_LIMIT) { m_currentSpeed.z = -SPEED_LIMIT; }
	if (m_currentSpeed.x > SPEED_LIMIT) { m_currentSpeed.x = SPEED_LIMIT; }
	if (m_currentSpeed.y > SPEED_LIMIT) { m_currentSpeed.y = SPEED_LIMIT; }
	if (m_currentSpeed.z > SPEED_LIMIT) { m_currentSpeed.z = SPEED_LIMIT; }

	//�ʏ푬�x�����Z����
	m_nextPos += m_currentSpeed;
	//�ړ��Ɋւ�����͂��Ȃ��Ă����̃t���[�����������������������ߏ��Z
	m_currentSpeed /= 1.05f;
}

Vector3 PlayerMove::Execute(Vector3 currentPos)
{
	//�ʏ�ړ�
	Move(currentPos);
	//�_�b�V���g�p���̈ړ�
	Dash();

	//�_�b�V������߂��珙�X�Ɍ���
	if (!g_pad[0]->IsPress(enButtonRB1))
	{
		m_isDash = false;
		//�_�b�V���̉e���x�����炵�Ă���
		m_dashSpeedRate /= 1.04f;
		//�_�b�V���̉e�����Ȃ��Ȃ�΁A�����Ɋւ���p�����[�^���ŏ��̏�Ԃɖ߂��B
		if (m_dashSpeedRate <= 1.0f)
		{
			m_isDecayDash = false;
			m_dashSpeedRate = 1.0f;
			count = 0.0f;
		}
	}

	//�v�Z�ς݂̈ʒu���W��Ԃ�
	return m_nextPos;
}

Vector3 PlayerMove::CalcPlayerPos(Vector3 homePos)
{
	//�������͂��Ă��Ȃ��Ƃ��̍��W���z�[���|�W�V����
	Vector3 playerPos = homePos;
	
	//�_�b�V�����Ȃ�����Ƃ��炵�A���񂾂񌳂̈ʒu��
	if (m_isDash)
	{
		//�\���ɍŏ��̉��������A�J�����ƃv���C���[�̈ʒu��������x���킹�ɍs����ԂȂ��
		if (!m_isDecayDash) {
			//�_�b�V���ɂ���Ă��񂾂��������B
			m_dashSpeedRate += 0.2f;
			//��萔�ȏ�ɉ���������
			if (m_dashSpeedRate >= 2.5f)
			{
				//��莞�Ԃ��̑��x���L�[�v
				count += GameTime().GetFrameDeltaTime();
				
				if (count >= 1.0f)
				{
					//���X�ɃJ�����ʒu�����ɖ߂��t���O�����Ă�
					m_isDecayDash = true;
					count = 0.0f;
				}
				m_dashSpeedRate = 2.5f;
			}
		}
		//�J�����ʒu�����ɖ߂��^�C�~���O�Ȃ�
		else if(m_isDecayDash == true)
		{
			//���������Ă���
			m_dashSpeedRate -= 0.005f;
			if (m_dashSpeedRate <= 1.0f)
			{
				m_dashSpeedRate = 1.0f;
			}
		}
	}
	//�z�[���|�W�V�������炸�炵�����ړ���
	Vector3 plusSpeed = m_currentSpeed * m_dashSpeedRate;



	//��������֐��ɒ����Ă����������s���S
	/*Vector3 kariPlusSpeed = plusSpeed;
	kariPlusSpeed.Normalize();
	m_prevPlusSpeed.Normalize();

	if (m_prevPlusSpeed.Dot(kariPlusSpeed) < 0.0f)
	{
		plusSpeed = m_currentSpeed * m_dashSpeedRate * 20000.0f;
	};*/




	//Y���W�̈ړ��ʂ𒲐�
	plusSpeed.y /= 1.3f;
	//�O�t���[���̉������x��ۑ�
	m_prevPlusSpeed = plusSpeed;
	//�v���C���[�̈ʒu���W�ɉ��Z
	playerPos += plusSpeed;

	return playerPos;
}