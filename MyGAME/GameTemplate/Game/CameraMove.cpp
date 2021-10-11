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
	g_camera3D->RotateOriginTarget(rotY);

	//プレイヤーからカメラへのベクトルを求める
	Vector3 playerPosToCamera = currentPlayerPos - g_camera3D->GetPosition();

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF());
	//ベクトルにY軸における回転を与える。
	qRot.Apply(playerPosToCamera);

	//X軸
	Vector3 axisX;
	//外積を用いてX軸として利用するベクトルを求める
	axisX.Cross(Vector3::AxisY, playerPosToCamera);
	axisX.Normalize();
	//X軸周りに回転させる。
	qRot.SetRotationDeg(axisX, -g_pad[0]->GetRStickYF());
	//ベクトルに適用する。
	qRot.Apply(playerPosToCamera);

	Vector3 toPosDir = playerPosToCamera;
	toPosDir.Normalize();
	if (toPosDir.y < -0.99f) {
		return;
	}
	else if (toPosDir.y > 0.9f) {
		return;
	}

	g_camera3D->RotateOriginTarget(qRot);
}


void CameraMove::Translation(Vector3 prevPlayerPos, Vector3 currentPlayerPos)
{
	//1フレーム前のプレイヤーの位置と現在の位置を減算して、移動量を計測する。
	Vector3 translationVol = currentPlayerPos - prevPlayerPos;
	//カメラターゲットをプレイヤーの移動分だけ平行移動する。
	Vector3 newCameraTarget = g_camera3D->GetTarget() + translationVol;
	Vector3 newCameraPos = g_camera3D->GetPosition() + translationVol;
	g_camera3D->SetTarget(newCameraTarget);
	g_camera3D->SetPosition(newCameraPos);
	g_camera3D->Update();

}

void CameraMove::UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentPlayerPos)
{
	//新しいカメラのターゲットを計算
	UpdateCameraTarget(currentPlayerPos);

	//カメラの平行移動を行う。
	Translation(prevPlayerPos, currentPlayerPos);
}