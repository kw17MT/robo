#pragma once
class MissileBallistic : public IGameObject
{
private:
	Vector3 m_cameraForward = Vector3::Zero;		
	bool m_return = false;
	float senkaiTime = 1.0f;
	float m_cosVol = 0.0f;
	float m_sinVol = 180.0f;

	enum EnBallisticType
	{
		enStraight,
		enMeandeling,
		enRolling,
		enTypeNum
	};
public:
	/**
	 * @brief �~�T�C�������܂ꂽ���_�ł̃J�����̑O������ݒ肷��
	 * @param cameraForward �J�����̑O����
	*/
	void SetCameraForward(Vector3 cameraForward)
	{
		m_cameraForward = cameraForward;
	}

	/**
	 * @brief �~�T�C���̒e���^�C�v�����߂�
	*/
	void DecideBallisticType();

	/**
	 * @brief �e���̕������v�Z���ĕԂ�
	 * @return �e���̕���
	*/
	Vector3 GetBallisticDirection();

	void ProgressDegreeChange();
private:
	EnBallisticType m_type = enStraight;
};

