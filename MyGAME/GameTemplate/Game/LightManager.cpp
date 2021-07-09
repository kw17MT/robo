#include "stdafx.h"
#include "LightManager.h"

LightManager::LightManager()
{
	//ディレクションライトの正規化
	s_allLight.directionalLight.directionalDirection.Normalize();
	//s_allLight.directionalDirection.Normalize();
	//視点（メインカメラの位置）
	s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
	//s_allLight.eyePos = g_camera3D->GetPosition();
	//スポットライト2種類の初期化、正規化
	s_allLight.spotDirection.Normalize();

	/*s_allLight.spotLight0.spotPosition = { 500.0f, 100.0f, 0.0f };
	s_allLight.spotLight0.spotDirection = { 1.0f, -1.0f, 0.0 };
	s_allLight.spotLight0.spotDirection.Normalize();
	s_allLight.spotLight0.spotColor = { 0.3f,0.0f,0.0f };*/

	/*s_allLight.spotLight1.spotPosition = { -500.0f,100.0f,0.0f };
	s_allLight.spotLight1.spotDirection = { -1.0f, -1.0f, 0.0 };
	s_allLight.spotLight1.spotDirection.Normalize();
	s_allLight.spotLight1.spotColor = { 0.0f,0.0f,0.3f };*/
}
