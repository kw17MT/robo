#include "stdafx.h"
#include "LightManager.h"

LightManager* LightManager::instance = nullptr;

LightManager::LightManager()
{
	//ディレクションライトの正規化
	s_allLight.directionalLight.directionalDirection.Normalize();
	TurnOnSpotLight();
}

void LightManager::TurnOnSpotLight()
{
	//視点（メインカメラの位置）
	s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
	//スポットライト1個目の初期化（赤色）
	s_allLight.spotLight[0].spotPosition = { 60.0f, 300.0f, 4600.0f };
	s_allLight.spotLight[0].spotDirection = { -1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[0].spotDirection.Normalize();
	s_allLight.spotLight[0].spotColor = { 0.25f,0.25f,0.75f };
	//スポットライト2個目の初期化（青色）
	s_allLight.spotLight[1].spotPosition = { -60.0f,300.0f, 4600.0f };
	s_allLight.spotLight[1].spotDirection = { 1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[1].spotDirection.Normalize();
	s_allLight.spotLight[1].spotColor = { 0.25f,0.25f, 0.75f };


	s_allLight.spotLight[2].spotPosition = { 60.0f,300.0f, 4200.0f };
	s_allLight.spotLight[2].spotDirection = { -1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[2].spotDirection.Normalize();
	s_allLight.spotLight[2].spotColor = { 0.25f,0.25f, 0.75f };

	s_allLight.spotLight[3].spotPosition = { -60.0f,300.0f, 4200.0f };
	s_allLight.spotLight[3].spotDirection = { 1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[3].spotDirection.Normalize();
	s_allLight.spotLight[3].spotColor = { 0.25f,0.25f, 0.75f };

	s_allLight.spotLight[4].spotPosition = { 60.0f,300.0f, 3800.0f };
	s_allLight.spotLight[4].spotDirection = { -1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[4].spotDirection.Normalize();
	s_allLight.spotLight[4].spotColor = { 0.25f,0.25f, 0.75f };

	s_allLight.spotLight[5].spotPosition = { -60.0f,300.0f, 3800.0f };
	s_allLight.spotLight[5].spotDirection = { 1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[5].spotDirection.Normalize();
	s_allLight.spotLight[5].spotColor = { 0.25f,0.25f, 0.75f };

	s_allLight.spotLight[6].spotPosition = { 60.0f,300.0f, 3200.0f };
	s_allLight.spotLight[6].spotDirection = { -1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[6].spotDirection.Normalize();
	s_allLight.spotLight[6].spotColor = { 0.25f,0.25f, 0.75f };

	s_allLight.spotLight[7].spotPosition = { -60.0f,300.0f, 3200.0f };
	s_allLight.spotLight[7].spotDirection = { 1.0f, 1.0f, 0.0 };
	s_allLight.spotLight[7].spotDirection.Normalize();
	s_allLight.spotLight[7].spotColor = { 0.25f,0.25f, 0.75f };
}