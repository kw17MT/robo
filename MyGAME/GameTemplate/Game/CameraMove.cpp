#include "stdafx.h"
#include "CameraMove.h"

namespace
{
	const float CAMERA_TO_PLAYER_SPACE = 3000.0f;
	const float AJUST_CAMERA_Y = 2000.0f;
}

void CameraMove::UpdateCameraTarget(Vector3 currentCameraPos)
{
	//右スティックの入力具合に応じて現在のカメラの位置を基点に回転させる。
	Quaternion rotX;
	rotX.SetRotationDegX(g_pad[0]->GetRStickXF());
	Quaternion rotY;
	rotY.SetRotationDegY(g_pad[0]->GetRStickYF());
	g_camera3D->RotateOriginCurrentPos(rotX);
	g_camera3D->RotateOriginCurrentPos(rotY);
}

Vector3 CameraMove::UpdateCameraPos(Vector3 currentPlayerPos)
{
	//現在のカメラターゲットからプレイヤーへのベクトル
	Vector3 targetToPlayerVec = currentPlayerPos - g_camera3D->GetTarget();
	//正規化して大きさを1にする
	targetToPlayerVec.Normalize();
	//プレイヤーとターゲット座標が常に画面に映るように。
	Vector3 newCameraPos = currentPlayerPos + targetToPlayerVec * CAMERA_TO_PLAYER_SPACE;
	//カメラの位置を上に調節する。
	newCameraPos.y += AJUST_CAMERA_Y;
	g_camera3D->SetPosition(newCameraPos);

	return newCameraPos;
}

void CameraMove::Translation(Vector3 prevPlayerPos, Vector3 currentPlayerPos)
{
	//1フレーム前のプレイヤーの位置と現在の位置を減算して、移動量を計測する。
	Vector3 translationVol = currentPlayerPos - prevPlayerPos;
	//カメラターゲットをプレイヤーの移動分だけ平行移動する。
	Vector3 newCameraTarget = g_camera3D->GetTarget() + translationVol;

	g_camera3D->SetTarget(newCameraTarget);
}

void CameraMove::UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentPlayerPos)
{
	//新しいカメラの位置を取得
	Vector3 cameraPos = UpdateCameraPos(currentPlayerPos);
	//新しいカメラのターゲットを計算
	UpdateCameraTarget(cameraPos);
	//カメラの平行移動を行う。
	Translation(prevPlayerPos, currentPlayerPos);
}