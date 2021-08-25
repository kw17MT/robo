#include "stdafx.h"
#include "LightManager.h"

LightManager::LightManager()
{
	//ディレクションライトの正規化
	s_allLight.directionalLight.directionalDirection.Normalize();
	//視点（メインカメラの位置）
	s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
	//スポットライト1個目の初期化（赤色）
	s_allLight.spotLight[0].spotPosition = { 1500.0f, 1200.0f, 0.0f };
	s_allLight.spotLight[0].spotDirection = { -1.0f, -1.0f, 0.0 };
	s_allLight.spotLight[0].spotDirection.Normalize();
	s_allLight.spotLight[0].spotColor = { 0.5f,0.1f,0.1f };
	//スポットライト2個目の初期化（青色）
	s_allLight.spotLight[1].spotPosition = { -1500.0f,1200.0f, 0.0f };
	s_allLight.spotLight[1].spotDirection = { 1.0f, -1.0f, 0.0 };
	s_allLight.spotLight[1].spotDirection.Normalize();
	s_allLight.spotLight[1].spotColor = { 0.1f,0.1f, 0.8f };
}

void LightManager::MakeGaugeLightYellow(int lightNumber)
{
	s_gaugeLight[lightNumber].ambient.y = 1.0f;
	s_gaugeLight[lightNumber].ambient.x = 1.0f;
}

void LightManager::MakeGaugeLightRed(int lightNumber)
{
	s_gaugeLight[lightNumber].ambient.y = 0.0f;
	s_gaugeLight[lightNumber].ambient.x = 1.0f;
}

void LightManager::MakeGaugeLightGreen(int lightNumber)
{
	s_gaugeLight[lightNumber].ambient = { 0.0f,1.0f,0.0f };
}
