#include "stdafx.h"
#include "CameraMove.h"

namespace
{
	const float CAMERA_TO_PLAYER_SPACE = 3000.0f;
	const float AJUST_CAMERA_Y = 2000.0f;
}

void CameraMove::UpdateCameraTarget(Vector3 currentHomePos)
{
	Quaternion rotY;
	rotY.SetRotationDegY(g_pad[0]->GetRStickXF() * 1.2f);
	g_camera3D->RotateOriginTarget(rotY);

	//�v���C���[����J�����ւ̃x�N�g�������߂�
	Vector3 playerPosToCamera = currentHomePos - g_camera3D->GetPosition();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF() * 1.2f);
	//�x�N�g����Y���ɂ������]��^����B
	qRot.Apply(playerPosToCamera);

	//X��
	Vector3 axisX;
	//�_�b�V�����̎��_�ύX�̋t�]���C��
	if (g_camera3D->GetForward().Dot(playerPosToCamera) < 0.0f) {
		playerPosToCamera *= -1.0f;
	}
	//�O�ς�p����X���Ƃ��ė��p����x�N�g�������߂�
	axisX.Cross(Vector3::AxisY, playerPosToCamera);
	axisX.Normalize();
	//X������ɉ�]������B
	qRot.SetRotationDeg(axisX, -g_pad[0]->GetRStickYF() * 1.2f);
	//�x�N�g���ɓK�p����B
	qRot.Apply(playerPosToCamera);

	Vector3 toPosDir = playerPosToCamera;
	toPosDir.Normalize();
	if (toPosDir.y < -0.95f) {
		return;
	}
	else if (toPosDir.y > 0.95f) {
		return;
	}

	m_prevPos = g_camera3D->GetPosition();
	m_prevRot = qRot;
	g_camera3D->RotateOriginTarget(qRot);
}


void CameraMove::Translation(Vector3 prevPlayerPos, Vector3 currentHomePos)
{
	//1�t���[���O�̃v���C���[�̈ʒu�ƌ��݂̈ʒu�����Z���āA�ړ��ʂ��v������B
	Vector3 translationVol = currentHomePos - prevPlayerPos;
	
	//�J�����^�[�Q�b�g���v���C���[�̈ړ����������s�ړ�����B
	Vector3 newCameraTarget = g_camera3D->GetTarget() + translationVol;
	Vector3 newCameraPos = g_camera3D->GetPosition() + translationVol;

	g_camera3D->SetTarget(newCameraTarget);
	g_camera3D->SetPosition(newCameraPos);
	g_camera3D->Update();
}

void CameraMove::UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentHomePos)
{
	//�J�����̕��s�ړ����s���B
	Translation(prevPlayerPos, currentHomePos);

	//�V�����J�����̃^�[�Q�b�g���v�Z
	UpdateCameraTarget(currentHomePos);
}