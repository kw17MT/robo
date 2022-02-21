#include "stdafx.h"
#include "LightManager.h"

LightManager* LightManager::instance = nullptr;

namespace
{
	const Vector3 SPOT_LIGHT_COLOR = { 0.25f,0.25f,0.75f };
	const Vector3 SPOT_LIGHT_DIRECTION_RIGHT = { -1.0f, 1.0f, 0.0 };
	const Vector3 SPOT_LIGHT_DIRECTION_LEFT = { 1.0f, 1.0f, 0.0 };
}

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
	s_allLight.spotLight[0].spotDirection = SPOT_LIGHT_DIRECTION_RIGHT;
	s_allLight.spotLight[0].spotDirection.Normalize();
	s_allLight.spotLight[0].spotColor = SPOT_LIGHT_COLOR;
	//スポットライト2個目の初期化（青色）
	s_allLight.spotLight[1].spotPosition = { -60.0f,300.0f, 4600.0f };
	s_allLight.spotLight[1].spotDirection = SPOT_LIGHT_DIRECTION_LEFT;
	s_allLight.spotLight[1].spotDirection.Normalize();
	s_allLight.spotLight[1].spotColor = SPOT_LIGHT_COLOR;


	s_allLight.spotLight[2].spotPosition = { 60.0f,300.0f, 4200.0f };
	s_allLight.spotLight[2].spotDirection = SPOT_LIGHT_DIRECTION_RIGHT;
	s_allLight.spotLight[2].spotDirection.Normalize();
	s_allLight.spotLight[2].spotColor = SPOT_LIGHT_COLOR;

	s_allLight.spotLight[3].spotPosition = { -60.0f,300.0f, 4200.0f };
	s_allLight.spotLight[3].spotDirection = SPOT_LIGHT_DIRECTION_LEFT;
	s_allLight.spotLight[3].spotDirection.Normalize();
	s_allLight.spotLight[3].spotColor = SPOT_LIGHT_COLOR;

	s_allLight.spotLight[4].spotPosition = { 60.0f,300.0f, 3800.0f };
	s_allLight.spotLight[4].spotDirection = SPOT_LIGHT_DIRECTION_RIGHT;
	s_allLight.spotLight[4].spotDirection.Normalize();
	s_allLight.spotLight[4].spotColor = SPOT_LIGHT_COLOR;

	s_allLight.spotLight[5].spotPosition = { -60.0f,300.0f, 3800.0f };
	s_allLight.spotLight[5].spotDirection = SPOT_LIGHT_DIRECTION_LEFT;
	s_allLight.spotLight[5].spotDirection.Normalize();
	s_allLight.spotLight[5].spotColor = SPOT_LIGHT_COLOR;

	s_allLight.spotLight[6].spotPosition = { 60.0f,300.0f, 3200.0f };
	s_allLight.spotLight[6].spotDirection = SPOT_LIGHT_DIRECTION_RIGHT;
	s_allLight.spotLight[6].spotDirection.Normalize();
	s_allLight.spotLight[6].spotColor = SPOT_LIGHT_COLOR;

	s_allLight.spotLight[7].spotPosition = { -60.0f,300.0f, 3200.0f };
	s_allLight.spotLight[7].spotDirection = SPOT_LIGHT_DIRECTION_LEFT;
	s_allLight.spotLight[7].spotDirection.Normalize();
	s_allLight.spotLight[7].spotColor = SPOT_LIGHT_COLOR;
}