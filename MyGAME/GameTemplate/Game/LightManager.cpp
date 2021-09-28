#include "stdafx.h"
#include "LightManager.h"

LightManager* LightManager::instance = nullptr;

LightManager::LightManager()
{
	//�f�B���N�V�������C�g�̐��K��
	s_allLight.directionalLight.directionalDirection.Normalize();
	//���_�i���C���J�����̈ʒu�j
	s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
	//�X�|�b�g���C�g1�ڂ̏������i�ԐF�j
	s_allLight.spotLight[0].spotPosition = { 1000.0f, 1200.0f, 0.0f };
	s_allLight.spotLight[0].spotDirection = { -1.0f, -1.0f, 0.0 };
	s_allLight.spotLight[0].spotDirection.Normalize();
	s_allLight.spotLight[0].spotColor = { 0.5f,0.1f,0.1f };
	//�X�|�b�g���C�g2�ڂ̏������i�F�j
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
