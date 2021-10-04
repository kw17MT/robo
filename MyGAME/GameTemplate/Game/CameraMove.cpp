#include "stdafx.h"
#include "CameraMove.h"

namespace
{
	const float CAMERA_TO_PLAYER_SPACE = 3000.0f;
	const float AJUST_CAMERA_Y = 2000.0f;
}

void CameraMove::UpdateCameraTarget(Vector3 currentCameraPos)
{
	//�E�X�e�B�b�N�̓��͋�ɉ����Č��݂̃J�����̈ʒu����_�ɉ�]������B
	Quaternion rotX;
	rotX.SetRotationDegX(g_pad[0]->GetRStickXF());
	Quaternion rotY;
	rotY.SetRotationDegY(g_pad[0]->GetRStickYF());
	g_camera3D->RotateOriginCurrentPos(rotX);
	g_camera3D->RotateOriginCurrentPos(rotY);
}

Vector3 CameraMove::UpdateCameraPos(Vector3 currentPlayerPos)
{
	//���݂̃J�����^�[�Q�b�g����v���C���[�ւ̃x�N�g��
	Vector3 targetToPlayerVec = currentPlayerPos - g_camera3D->GetTarget();
	//���K�����đ傫����1�ɂ���
	targetToPlayerVec.Normalize();
	//�v���C���[�ƃ^�[�Q�b�g���W����ɉ�ʂɉf��悤�ɁB
	Vector3 newCameraPos = currentPlayerPos + targetToPlayerVec * CAMERA_TO_PLAYER_SPACE;
	//�J�����̈ʒu����ɒ��߂���B
	newCameraPos.y += AJUST_CAMERA_Y;
	g_camera3D->SetPosition(newCameraPos);

	return newCameraPos;
}

void CameraMove::Translation(Vector3 prevPlayerPos, Vector3 currentPlayerPos)
{
	//1�t���[���O�̃v���C���[�̈ʒu�ƌ��݂̈ʒu�����Z���āA�ړ��ʂ��v������B
	Vector3 translationVol = currentPlayerPos - prevPlayerPos;
	//�J�����^�[�Q�b�g���v���C���[�̈ړ����������s�ړ�����B
	Vector3 newCameraTarget = g_camera3D->GetTarget() + translationVol;

	g_camera3D->SetTarget(newCameraTarget);
}

void CameraMove::UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentPlayerPos)
{
	//�V�����J�����̈ʒu���擾
	Vector3 cameraPos = UpdateCameraPos(currentPlayerPos);
	//�V�����J�����̃^�[�Q�b�g���v�Z
	UpdateCameraTarget(cameraPos);
	//�J�����̕��s�ړ����s���B
	Translation(prevPlayerPos, currentPlayerPos);
}