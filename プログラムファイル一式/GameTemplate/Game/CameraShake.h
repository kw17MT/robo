#pragma once
class CameraShake
{
private:
	float m_duration = 0.0f;
	float m_strength = 0.0f;								
	float m_shakingDuration = 0.0f;

	float a = 0.0f;

	Vector3 m_positionToReturn = Vector3::Zero;
	Vector3 m_moveDirection = Vector3::Zero;

	bool m_isFinishShake = true;
	bool m_shouldHarder = false;
	bool m_isAwayFromHome = false;
	bool m_canShake = true;

	enum EnDirection
	{
		enLeftUp,
		enLeftMiddle,
		enLeftBottom,
		enUp,
		enBottom,
		enRightUp,
		enRightMiddle,
		enRightBottom
	};
public:
	/**
	 * @brief �h��ɕK�v�ȃp�����[�^��ݒ肷��
	 * @param duration �p������
	 * @param strength �h��̋���
	 * @param shouldHarder ���Ԃɂ���ėh���i�X�������邩
	*/
	void SetParam(float duration, float strength, bool shouldHarder)
	{
		m_duration = duration;
		m_strength = strength;
		m_shouldHarder = shouldHarder;
		m_isFinishShake = false;
	}

	/**
	 * @brief �߂�ɍs���ʒu���W��ݒ肷��
	 * �h����g���ꍇ�A���t���[���ݒ肷�邱��
	 * @param pos �߂�ɍs���ʒu
	*/
	void SetHomePosition(Vector3 pos) { m_positionToReturn = pos; }

	/**
	 * @brief �h���������v�Z����
	 * @return �h��̕���
	*/
	Vector3 Shake()
	{
		//�h�炷�̂��I����Ă���Ȃ�΁A�ȉ��͖���
		if (m_isFinishShake)
		{
			return Vector3::Zero;
		}

		//���̈ʒu���牓�������Ă��Ȃ����
		if (!m_isAwayFromHome && m_canShake)
		{
			int direction = rand() % 9;
			m_isAwayFromHome = true;
			m_canShake = false;

			direction = 0;
			switch (direction)
			{
			case enLeftUp:
				m_moveDirection = (g_camera3D->GetRight() * -1.0f) + g_camera3D->GetUp();
				m_moveDirection.Normalize();
				m_moveDirection = m_moveDirection * m_strength;
				
				break;

			case enLeftMiddle:
				break;
			default:
				break;
			}
		}
		else if (m_isAwayFromHome && !m_canShake)
		{
			m_moveDirection = m_positionToReturn - g_camera3D->GetPosition();
			Vector3 length = m_moveDirection;
			m_moveDirection.Normalize();
			m_moveDirection *= m_strength;
			if (a = length.Length() < 1.0f)
			{
				m_canShake = true;
			}
		}

		m_shakingDuration += GameTime().GetFrameDeltaTime();
		if (m_shakingDuration >= 0.5f)
		{
			m_shakingDuration = 0.0f;
			m_isAwayFromHome = false;
		}

		//�i�X���������Ă����Ȃ��
		if (m_shouldHarder)
		{
			//�p�����Ԃ������ȏ゠��Ȃ��
			if (m_duration >= m_duration / 2.0f)
			{
				//�h��̋����𖈃t���[����������
				m_strength += m_strength / 100.0f;
			}
			//�p�����Ԃ��c�蔼���ȉ��Ȃ��
			else
			{
				//�h������߂Ă���
				m_strength -= m_strength / 100.0f;
			}
		}

		//�p�����Ԃ��f���^�^�C���Ō��炵�Ă���
		m_duration -= GameTime().GetFrameDeltaTime();
		//�p�����Ԃ��Ȃ��Ȃ��
		if (m_duration <= 0.0f)
		{
			//�h�ꂨ���
			m_isFinishShake = true;
			//�h��Ɋւ���p�����[�^��������
			SetParam(0.0f, 0.0f, false);
		}

		return m_moveDirection;
	}
};