#pragma once
class CameraMove
{
private:
	bool m_isSetDeadCamera = false;						//�v���C���[���|���ꂽ���̃J�����̈ʒu���v�Z���邩�ǂ���
	Vector3 m_prevCameraPos = Vector3::Zero;			//1�t���[���O�̃J�����̈ʒu
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

	/**
	 * @brief �v���C���[���|���ꂽ���̃J�����̈ʒu�v�Z�֐�
	 * @param prevMoveDirection 1�t���[���O�̃v���C���[�̈ړ�����
	*/
	void SetDeadCamera(Vector3 prevMoveDirection);

	/**
	 * @brief �v���C���[���|���ꂽ���̃J�����̈ʒu���v�Z���邩�ǂ����ݒ肷��
	 * @param state TRUE�@�v�Z����@FALSE�@�v�Z���Ȃ�
	*/
	void SetIsDeadCamera(bool state) { m_isSetDeadCamera = state; }
};

