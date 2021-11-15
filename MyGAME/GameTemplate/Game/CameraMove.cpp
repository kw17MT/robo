#include "stdafx.h"
#include "CameraMove.h"

namespace
{
	const float CAMERA_TO_PLAYER_SPACE = 3000.0f;			//カメラとプレイヤーとの間の長さ
}

void CameraMove::UpdateCameraTarget(Vector3 currentHomePos)
{
	Quaternion rotY;
	rotY.SetRotationDegY(g_pad[0]->GetRStickXF() * 1.2f);
	g_camera3D->RotateOriginTarget(rotY);

	//プレイヤーからカメラへのベクトルを求める
	Vector3 playerPosToCamera = currentHomePos - g_camera3D->GetPosition();

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, g_pad[0]->GetRStickXF() * 1.2f);
	//ベクトルにY軸における回転を与える。
	qRot.Apply(playerPosToCamera);

	//X軸
	Vector3 axisX;
	//ダッシュ中の視点変更の逆転を修正
	if (g_camera3D->GetForward().Dot(playerPosToCamera) < 0.0f) {
		playerPosToCamera *= -1.0f;
	}
	//外積を用いてX軸として利用するベクトルを求める
	axisX.Cross(Vector3::AxisY, playerPosToCamera);
	axisX.Normalize();
	//X軸周りに回転させる。
	qRot.SetRotationDeg(axisX, -g_pad[0]->GetRStickYF() * 1.2f);
	//ベクトルに適用する。
	qRot.Apply(playerPosToCamera);

	//回転の抑制
	Vector3 toPosDir = playerPosToCamera;
	toPosDir.Normalize();
	//下回転の抑制
	if (toPosDir.y < -0.95f) {
		return;
	}
	//上回転の抑制
	else if (toPosDir.y > 0.95f) {
		return;
	}

	//1フレーム前のカメラの位置として保存
	m_prevCameraPos = g_camera3D->GetPosition();
	g_camera3D->RotateOriginTarget(qRot);
}


void CameraMove::Translation(Vector3 prevPlayerPos, Vector3 currentHomePos)
{
	//1フレーム前のプレイヤーの位置と現在の位置を減算して、移動量を計測する。
	Vector3 translationVol = currentHomePos - prevPlayerPos;
	
	//カメラターゲットをプレイヤーの移動分だけ平行移動する。
	Vector3 newCameraTarget = g_camera3D->GetTarget() + translationVol;
	Vector3 newCameraPos = g_camera3D->GetPosition() + translationVol;

	g_camera3D->SetTarget(newCameraTarget);
	g_camera3D->SetPosition(newCameraPos);
	g_camera3D->Update();
}

void CameraMove::UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentHomePos)
{
	//カメラの平行移動を行う。
	Translation(prevPlayerPos, currentHomePos);

	//新しいカメラのターゲットを計算
	UpdateCameraTarget(currentHomePos);
}

void CameraMove::SetDeadCamera(Vector3 prevMoveDirection)
{
	//プレイヤー死亡時のカメラを設定するなら
	if (m_isSetDeadCamera)
	{
		//プレイヤーの1フレーム前の移動方向を取得
		Vector3 direction = prevMoveDirection;
		//正規化
		direction.Normalize();
		//y方向は不要
		direction.y = 0.0f;
		Quaternion qRot;
		//Y軸基準で160度回転を作成
		qRot.SetRotationDegY(160.0f);
		//プレイヤーの進行方向に適用
		qRot.Apply(direction);
		//y方向を修正
		direction.y += 0.5f;
		direction *= 500.0f;
		//最終的に適用する位置を設定
		Vector3 pos = m_prevCameraPos + direction;
		g_camera3D->SetPosition(pos);

		//一度だけ設定する
		m_isSetDeadCamera = false;
	}
}