#include "stdafx.h"
#include "Calculation.h"


void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos)
{
	//対応する敵のポジションからスクリーン座標を求める
	g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, pos);
	screenPos.x *= -1.0f;
	//敵の位置とカメラの前方向の内積によってアイコンを映すか決める
	//敵からカメラへのベクトル作成
	Vector3 targetToCameraVec = g_camera3D->GetPosition() - pos;
	//正規化
	targetToCameraVec.Normalize();
	//敵の位置とカメラの前方向の内積
	float dot = g_camera3D->GetForward().Dot(targetToCameraVec);
	//敵がカメラの前方向にあるならば映す
	if (dot < 0.0f)
	{
		screenPos.z = 0.0f;
	}
	//後ろ側にある
	else
	{
		screenPos.z = -1.0f;
	}
}

float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2)
{
	Vector3 distanceVec = v1 - v2;
	float distance = distanceVec.Length();

	return distance;
}