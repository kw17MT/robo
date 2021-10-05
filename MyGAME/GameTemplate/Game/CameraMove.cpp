#include "stdafx.h"
#include "CameraMove.h"

namespace
{
	const float CAMERA_TO_PLAYER_SPACE = 3000.0f;
	const float AJUST_CAMERA_Y = 2000.0f;
}

void CameraMove::UpdateCameraTarget(Vector3 currentPlayerPos)
{
	Quaternion rotY;
	rotY.SetRotationDegY(g_pad[0]->GetRStickXF());
	g_camera3D->RotateOriginCurrentPos(rotY);

	//�v���C���[����J�����ւ̃x�N�g�������߂�
	Vector3 playerPosToCamera = currentPlayerPos - g_camera3D->GetPosition();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF());
	//�x�N�g����Y���ɂ������]��^����B
	qRot.Apply(playerPosToCamera);

	//X��
	Vector3 axisX;
	//�O�ς�p����X���Ƃ��ė��p����x�N�g�������߂�
	axisX.Cross(Vector3::AxisY, playerPosToCamera);
	axisX.Normalize();
	//X������ɉ�]������B
	qRot.SetRotationDeg(axisX, g_pad[0]->GetRStickYF());
	//�x�N�g���ɓK�p����B
	qRot.Apply(playerPosToCamera);

	Vector3 toPosDir = playerPosToCamera;
	toPosDir.Normalize();
	if (toPosDir.y < -0.99f) {
		return;
	}
	else if (toPosDir.y > 0.9f) {
		return;
	}

	g_camera3D->RotateOriginCurrentPos(qRot);
}

void CameraMove::UpdateCameraPos(Vector3 currentPlayerPos)
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
	//�V�����J�����̃^�[�Q�b�g���v�Z
	UpdateCameraTarget(currentPlayerPos);
	//�V�����J�����̈ʒu���擾
	UpdateCameraPos(currentPlayerPos);
	//�J�����̕��s�ړ����s���B
	Translation(prevPlayerPos, currentPlayerPos);
}