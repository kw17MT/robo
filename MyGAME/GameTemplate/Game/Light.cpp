#include "stdafx.h"
#include "Light.h"
#include "math.h"

//Light::Light()
//{
//	LightData
//	//ディレクションライト
//	DirDirection = { 1.0f,0.0f,0.0f };
//	DirDirection.Normalize();
//	DirColor = { 1.0f,1.0f,1.0f };
//	eyePos = g_camera3D->GetPosition();
//
//	//ポイントライト
//	ptPosition = { 0.0f, 60.0f,0.0f };
//	ptColor = { 0.0f, 100.0f,100.0f };
//	ptRange = 300.0f;
//
//	//スポットライト
//	spDirection = { 1.0f,-1.0f,0.0f };
//	spDirection.Normalize();
//
//	spAngle = Math::DegToRad(30.0f);
//}