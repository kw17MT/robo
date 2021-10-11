#include "stdafx.h"
#include "PlayerRotation.h"

Quaternion PlayerRotation::RotationX(Quaternion playerRot)
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

Quaternion PlayerRotation::RotationY(Quaternion playerRot)
{
	Vector3 roboRight = g_camera3D->GetRight();
	
	Quaternion qRot = playerRot;

	qRot.SetRotation(roboRight, -g_pad[0]->GetRStickYF() / 2.0f);

	Vector3 a = { 0.0f,1.0f,0.0f };
	//qRot.Apply(a);
	return qRot;
}

Quaternion PlayerRotation::RotationWithCamera()
{
	Quaternion qRot;

	qRot.SetRotation(g_camera3D->GetCameraRotation());

	if (g_pad[0]->GetLStickXF())
	{
		m_rotationAngle = g_pad[0]->GetLStickXF();
		qRot.AddRotationY(m_rotationAngle);
	}

	return qRot;
}

Quaternion PlayerRotation::ResetRotation()
{
	Quaternion qRot;
	qRot.SetRotation(g_camera3D->GetCameraRotation());

	m_rotationAngle /= 1.04f;
	qRot.AddRotationY(m_rotationAngle);
	return qRot;
}

void PlayerRotation::UpdateRotation(SkinModelRender* skinModel, Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front)
{
	if (g_pad[0]->GetLStickXF()) {
		skinModel->SetRotation(RotationWithCamera());
	}
	else
	{
		skinModel->SetRotation(ResetRotation());
	}
}