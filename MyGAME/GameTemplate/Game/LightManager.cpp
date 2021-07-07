#include "stdafx.h"
#include "LightManager.h"

LightManager::LightManager()
{
	//ディレクションライトの正規化
	s_allLight.directionalDirection.Normalize();
	//視点（メインカメラの位置）
	s_allLight.eyePos = g_camera3D->GetPosition();
}
