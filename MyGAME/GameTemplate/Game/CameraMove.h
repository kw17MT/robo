#pragma once
class CameraMove
{
private:
	bool m_isSetDeadCamera = false;
public:
	/**
	 * @brief �J�����̃^�[�Q�b�g���X�V����
	*/
	void UpdateCameraTarget(Vector3 currentHomePos);


	/**
	 * @brief �v���C���[�̈ړ��ɔ����J�����^�[�Q�b�g�̕��s�ړ�
	 * @param prevPlayerPos 1�t���[���O�̃v���C���[�̈ʒu
	 * @param currentPlayerPos ���݂̃v���C���[�̈ʒu
	*/
	void Translation(Vector3 prevPlayerPos, Vector3 currentHomePos);

	/**
	 * @brief �J�����̍X�V�����̂��ׂĂ��s��
	 * @param prevPlayerPos 1�t���[���O�̃v���C���[�̈ʒu
	 * @param currentPlayerPos ���݂̃v���C���[�̈ʒu
	*/
	void UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentHomePos);

	void SetDeadCamera(Vector3 prevMoveDirection);

	void SetSetDeadCamera(bool state) { m_isSetDeadCamera = state; }

private:
	Vector3 m_correctTarget = g_camera3D->GetTarget();
	Vector3 m_correctPos = g_camera3D->GetPosition();
	Vector3 m_prevPos = Vector3::Zero;
	Quaternion m_prevRot = Quaternion::Identity;
};

