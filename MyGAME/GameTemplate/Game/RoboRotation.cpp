#include "stdafx.h"
#include "RoboRotation.h"

Quaternion RoboRotation::RotationX(Quaternion playerRot)
{
	Vector3 roboUp = g_camera3D->GetUp();
	Vector3 roboForward = g_camera3D->GetForward();
	Quaternion currentRot = playerRot;

	Quaternion qRot;
	//���͗ʂ���]�ʂɕϊ�
	qRot.SetRotation(roboUp, g_pad[0]->GetRStickXF() / 20.0f);
	////1�t���[���O�̉�]�ʂɍ���̃t���[���œK�p�����]�ʂ���Z
	qRot.Multiply(playerRot);
	//�J�����̑O�����ɓK�p���A���{�������Ă�������x�N�g�������߂�B
	qRot.Apply(roboForward);
	roboForward.Normalize();
	//�J�����̑O�����ƁA���{�̌����Ă�������̓��ς��s���A�ǂꂾ���Ⴄ�����߂�
	//float a = 0.0f;
	//a = roboForward.x * g_camera3D->GetForward().x + roboForward.y * g_camera3D->GetForward().y/* + roboForward.z * g_camera3D->GetForward().z*/;
	////������܂ŉ�]�����炻��ȏ��]���Ȃ��悤��
	//if (a < 0.0f) {
	//	return currentRot;
	//
	return qRot;
}

Quaternion RoboRotation::RotationY(Quaternion playerRot)
{
	Vector3 roboRight = g_camera3D->GetRight();
	
	Quaternion qRot = playerRot;

	qRot.SetRotation(roboRight, -g_pad[0]->GetRStickYF() / 2.0f);

	Vector3 a = { 0.0f,1.0f,0.0f };
	//qRot.Apply(a);
	return qRot;
}
//


Quaternion RoboRotation::RotationWithCamera(Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front)
{
	//Vector3 moveDirection = currentMoveVec - prevMoveVec;
	//moveDirection.Normalize();
	//Vector3 cameraForward = g_camera3D->GetForward();


	////float angle = cosf(Dot(moveDirection, cameraForward) / abs(Dot(moveDirection) * abs(Dot(cameraForward)));
	//Quaternion qRot;
	//qRot.SetRotation(moveDirection, cameraForward);

	//return qRot;


	////�v���C���[�̑O�����ƃv���C���[����J�����^�[�Q�b�g�̃x�N�g�������߁A��]�����߂�
	//Vector3 playerToTarget = g_camera3D->GetTarget() - currentMoveVec;

	//Vector3 cameraToPlayer = currentMoveVec - g_camera3D->GetPosition();
	//cameraToPlayer.Normalize();
	//Vector3 playerToTarget = g_camera3D->GetPosition() - currentMoveVec;
	//playerToTarget.Normalize();
	////
	////float angle = 

	Quaternion qRot;
	//qRot.SetRotation(cameraToPlayer, playerToTarget);
	//return qRot;

	/*Vector3 up = g_camera3D->GetUp();
	Vector3 right = g_camera3D->GetRight();
	Vector3 forward = g_camera3D->GetForward();
	qRot.SetRotation(front, forward);
	qRot.Apply(front);
	return qRot;*/


	/*Vector3 roboFront = g_camera3D->GetForward();
	Vector3 roboRight = g_camera3D->GetRight();
	qRot.SetRotation(roboFront, roboRight);*/

	qRot.SetRotation(g_camera3D->GetCameraRotation());

	return qRot;
}

//Quaternion RoboRotation::ResetRotation(Quaternion playerRot)
//{
//
//}


void RoboRotation::UpdateRotation(SkinModelRender* skinModel, Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front)
{
	
	//skinModel->SetRotation(RotationY(skinModel->GetRotation()));
	//skinModel->SetRotation(RotationX(skinModel->GetRotation()));
	skinModel->SetRotation(RotationWithCamera(prevMoveVec, currentMoveVec, front));
}