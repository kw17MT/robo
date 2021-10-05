#pragma once
class CameraMove
{
public:
	/**
	 * @brief �J�����̃^�[�Q�b�g���X�V����
	*/
	static void UpdateCameraTarget(Vector3 currentPlayerPos);

	/**
	 * @brief �J�����̈ʒu���X�V����B
	 * @param playerPos ���݂̃v���C���[�̈ʒu
	 * @return 
	*/
	static void UpdateCameraPos(Vector3 currentPlayerPos);

	/**
	 * @brief �v���C���[�̈ړ��ɔ����J�����^�[�Q�b�g�̕��s�ړ�
	 * @param prevPlayerPos 1�t���[���O�̃v���C���[�̈ʒu
	 * @param currentPlayerPos ���݂̃v���C���[�̈ʒu
	*/
	static void Translation(Vector3 prevPlayerPos, Vector3 currentPlayerPos);

	/**
	 * @brief �J�����̍X�V�����̂��ׂĂ��s��
	 * @param prevPlayerPos 1�t���[���O�̃v���C���[�̈ʒu
	 * @param currentPlayerPos ���݂̃v���C���[�̈ʒu
	*/
	static void UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentPlayerPos);
};

