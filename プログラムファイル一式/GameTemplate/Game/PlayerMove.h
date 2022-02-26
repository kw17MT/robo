#pragma once
class PlayerMove
{
private:	
	Vector3 m_currentSpeed = Vector3::Zero;							//���݂̒ʏ�ړ��X�s�[�h
	Vector3 m_dashSpeed = Vector3::Zero;							//�_�b�V���������̃X�s�[�h
	Vector3 m_nextPos = Vector3::Zero;								//�e��X�s�[�h���l���������̃v���C���[�̍ŏI�ʒu
	Vector3 m_forwardDirection = g_camera3D->GetForward();			//��ʂ̑O��
	Vector3 m_sideDirection = g_camera3D->GetRight();				//��ʂ̉E����
	Vector3 m_upDirection = Vector3::Zero;							//��ʂ̏����

	bool m_isDash = false;
	float m_dashSpeedRate = 1.0f;
	bool m_isDecayDash = false;
	float count = 0.0f;

	bool m_canDash = true;

	Vector3 m_prevPlusSpeed = Vector3::Zero;

	float m_afterDeathTime = 0.0f;
	bool m_isSoundDash = false;
public:
	/**
	 * @brief R1�{�^�����͂ɂ��_�b�V���X�s�[�h�̌v�Z�֐�
	 * @return �_�b�V���X�s�[�h
	*/
	const void Dash();

	/**
	 * @brief �ʏ�̈ړ����x�̌v�Z�֐�
	 * @param currentPos �v���C���[�̌��݂̈ʒu
	 * @return �ʏ�̈ړ����x
	*/
	const void Move(const Vector3 currentPos);

	Vector3& GetMoveSpeed()
	{
		return m_prevPlusSpeed;
	}

	/**
	 * @brief �e�푬�x���l����̃v���C���[�̍ŏI�ʒu�̌v�Z�֐�
	 * @param currentPos �v���C���[�̌��݈ʒu
	 * @return �v���C���[�̍ŏI�ʒu
	*/
	Vector3 Execute(Vector3 currentPos);

	Vector3 CalcPlayerPos(Vector3 homePos);

	void SetCanDash(bool canDash) { m_canDash = canDash; }

	Vector3 DeadMove(Vector3 currentPos);

	bool GetIsDash() { return m_isDash; }
};