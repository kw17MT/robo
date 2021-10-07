#include "stdafx.h"
#include "RoboRotation.h"

Quaternion RoboRotation::RotationX(Quaternion playerRot)
{
	Vector3 roboUp = g_camera3D->GetUp();
	Vector3 roboForward = g_camera3D->GetForward();
	Quaternion currentRot = playerRot;

	Quaternion qRot;
	//入力量を回転量に変換
	qRot.SetRotation(roboUp, g_pad[0]->GetRStickXF() / 20.0f);
	////1フレーム前の回転量に今回のフレームで適用する回転量を乗算
	qRot.Multiply(playerRot);
	//カメラの前方向に適用し、ロボが向いている方向ベクトルを求める。
	qRot.Apply(roboForward);
	roboForward.Normalize();
	//カメラの前方向と、ロボの向いている方向の内積を行い、どれだけ違うか求める
	//float a = 0.0f;
	//a = roboForward.x * g_camera3D->GetForward().x + roboForward.y * g_camera3D->GetForward().y/* + roboForward.z * g_camera3D->GetForward().z*/;
	////ある一定まで回転したらそれ以上回転しないように
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


	////プレイヤーの前方向とプレイヤーからカメラターゲットのベクトルを求め、回転を求める
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