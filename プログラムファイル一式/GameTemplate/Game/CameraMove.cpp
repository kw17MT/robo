#include "stdafx.h"
#include "CameraMove.h"

namespace
{
	const float CAMERA_TO_PLAYER_SPACE = 3000.0f;			//�J�����ƃv���C���[�Ƃ̊Ԃ̒���
	const float DECTRICT_CAMERA_ROTATION_Y = 0.95f;
	const float DEAD_CAMERA_ROTATION_DEGREE = 160.0f;
	const float ADJUST_DIRCTION_Y = 0.5f;
	const float ROBO_CAMERA_SPACE = 500.0f;
}

void CameraMove::UpdateCameraTarget(Vector3 currentHomePos)
{
	Quaternion rotY;
	rotY.SetRotationDegY(g_pad[0]->GetRStickXF() * 2.0f);
	g_camera3D->RotateOriginTarget(rotY);

	//�v���C���[����J�����ւ̃x�N�g�������߂�
	Vector3 playerPosToCamera = currentHomePos - g_camera3D->GetPosition();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF() * 2.0f);
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
	qRot.SetRotationDeg(axisX, -g_pad[0]->GetRStickYF() * 2.0f);
	//�x�N�g���ɓK�p����B
	qRot.Apply(playerPosToCamera);

	//��]�̗}��
	Vector3 toPosDir = playerPosToCamera;
	toPosDir.Normalize();
	//����]�̗}��
	if (toPosDir.y < -DECTRICT_CAMERA_ROTATION_Y) {
		return;
	}
	//���]�̗}��
	else if (toPosDir.y > DECTRICT_CAMERA_ROTATION_Y) {
		return;
	}

	//1�t���[���O�̃J�����̈ʒu�Ƃ��ĕۑ�
	m_prevCameraPos = g_camera3D->GetPosition();
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

void CameraMove::SetDeadCamera(Vector3 prevMoveDirection)
{
	//�v���C���[���S���̃J������ݒ肷��Ȃ�
	if (m_isSetDeadCamera)
	{
		//�v���C���[��1�t���[���O�̈ړ��������擾
		Vector3 direction = prevMoveDirection;
		//���K��
		direction.Normalize();
		//y�����͕s�v
		direction.y = 0.0f;
		Quaternion qRot;
		//Y�����160�x��]���쐬
		qRot.SetRotationDegY(DEAD_CAMERA_ROTATION_DEGREE);
		//�v���C���[�̐i�s�����ɓK�p
		qRot.Apply(direction);
		//y�������C��
		direction.y += ADJUST_DIRCTION_Y;
		direction *= ROBO_CAMERA_SPACE;
		//�ŏI�I�ɓK�p����ʒu��ݒ�
		Vector3 pos = m_prevCameraPos + direction;
		g_camera3D->SetPosition(pos);

		//��x�����ݒ肷��
		m_isSetDeadCamera = false;
	}
}